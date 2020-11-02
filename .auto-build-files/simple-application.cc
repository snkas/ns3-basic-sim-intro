/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Simon
 * Adapted from UdpEchoServer/UdpEchoClient
 */

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "simple-application.h"

namespace ns3 {

    NS_LOG_COMPONENT_DEFINE ("SimpleApplicationApplication");

    NS_OBJECT_ENSURE_REGISTERED (SimpleApplication);

    TypeId
    SimpleApplication::GetTypeId(void) {
        static TypeId tid = TypeId("ns3::SimpleApplication")
                .SetParent<Application>()
                .SetGroupName("Applications")
                .AddConstructor<SimpleApplication>()
                .AddAttribute("Port", "Port on which we listen for incoming packets.",
                              UintegerValue(9),
                              MakeUintegerAccessor(&SimpleApplication::m_port),
                              MakeUintegerChecker<uint16_t>())
                .AddAttribute("RemoteAddress",
                              "The destination address of the outbound packet (IPv4 address, port)",
                              AddressValue(),
                              MakeAddressAccessor(&SimpleApplication::m_peerAddress),
                              MakeAddressChecker());
        return tid;
    }

    SimpleApplication::SimpleApplication() {
        NS_LOG_FUNCTION(this);
        m_socket = 0;
    }

    SimpleApplication::~SimpleApplication() {
        NS_LOG_FUNCTION(this);
        m_socket = 0;
    }

    void
    SimpleApplication::DoDispose(void) {
        NS_LOG_FUNCTION(this);
        Application::DoDispose();
    }

    void
    SimpleApplication::StartApplication(void) {
        NS_LOG_FUNCTION(this);
        if (m_socket == 0) {

            // Create socket
            TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
            m_socket = Socket::CreateSocket(GetNode(), tid);

            // Bind to local port (and any local IP address)
            InetSocketAddress local = InetSocketAddress(Ipv4Address::GetAny(), m_port);
            if (m_socket->Bind(local) == -1) {
                throw std::runtime_error("Failed to bind socket");
            }
            m_local = local;

        }

        // Callback for when the socket receives a packet
        m_socket->SetRecvCallback(MakeCallback(&SimpleApplication::HandleRead, this));

        // Log
        std::cout << Simulator::Now() << " -- Node " << this->GetNode()->GetId() << ": created UDP socket (IP="
                  << InetSocketAddress::ConvertFrom(m_local).GetIpv4() << " (which means any local IP address), port="
                  << InetSocketAddress::ConvertFrom(m_local).GetPort() << ") " << std::endl;

        // Now, we send one packet to the destination IP address/port combo we got provided

        // Packet with timestamp
        Ptr<Packet> packet = Create<Packet>();
        SeqTsHeader seqTs; // Creates one with the current timestamp
        seqTs.SetSeq(this->GetNode()->GetId());
        packet->AddHeader(seqTs);

        // Send out
        m_socket->SendTo(packet, 0, m_peerAddress);

        // Log
        std::cout << Simulator::Now() << " -- Node " << this->GetNode()->GetId() << ": sent out packet "
                  << seqTs << " to " << InetSocketAddress::ConvertFrom(m_peerAddress).GetIpv4() << ":" << InetSocketAddress::ConvertFrom(m_peerAddress).GetPort() << std::endl;

    }

    void
    SimpleApplication::StopApplication() { // Called at time specified by Stop
        if (m_socket != 0) {
            m_socket->Close();
            m_socket->SetRecvCallback(MakeNullCallback < void, Ptr < Socket > > ());
            m_socket = 0;
        }
    }

    void
    SimpleApplication::HandleRead(Ptr <Socket> socket) {
        NS_LOG_FUNCTION(this << socket);
        Ptr <Packet> packet;
        Address from;
        while ((packet = socket->RecvFrom(from))) {

            // Receiving header
            SeqTsHeader incomingSeqTs;
            packet->RemoveHeader (incomingSeqTs);

            // Log
            std::cout << Simulator::Now() << " -- Node " << this->GetNode()->GetId() << ": received packet " << incomingSeqTs
                      << " from " << InetSocketAddress::ConvertFrom(from).GetIpv4() << ":" << InetSocketAddress::ConvertFrom(from).GetPort() << std::endl;

        }
    }

} // Namespace ns3
