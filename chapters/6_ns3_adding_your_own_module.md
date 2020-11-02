# Chapter 6: Adding your own ns-3 module

In the previous chapter you became accustomed to creating main files
in the `scratch/` folder. Now, we are going to create a proper module instead.

## Creating the module

1. Go to the `ns-3` folder:

   ```
   cd /path/to/basic-sim-exercise/ns-3
   ```
   
2. Create your own module `contrib/simple-application` by executing:

   ```
   python utils/create-module.py simple-application
   ```

3. Now you can go into your `simple-application` module (`cd contrib/simple-application; ls`), and see the structure:

   ```
   ns-3
   |
   |-- contrib
       |
       |-- simple-application
           |
           |-- doc/
           |-- examples/
           |-- helper/
           |-- model/
           |-- test/
           |-- wscript
   ```
   
4. A module consists of these files/directories:

   - `doc/`
   
     Documentation files
     
   - `examples/`
   
     Example usage mains, it's like a private `scratch` of this module
     
   - `model/`
   
     C++ source and header files for core classes that are added to ns-3
     (let's say, a new `NetDevice` called `SuperNetDevice`)
     
   - `helper/`
   
     C++ source and header files for classes which help install these
     for users (in the example, `SuperNetDeviceHelper`). Helper classes CAN be used
     by users of this module, but they can also circumvent them and do everything
     themselves using directly the `model/` classes. Helper classes are added for
     convenience.
     
   - `wscript`
    
     File which described which other modules this module is dependent on,
     and explicitly declares all source and header files found in `model/` and `helper/`
     such that they are found by `waf` during compilation.


## Writing the module

1. In `wscript` you have to define ALL source and header files that are to be compiled.
   (the python command already added as "default" simple-application.cc/h and 
   simple-application-helper.cc/h as source/header files, so you don't have to do this
   in this example), and add `internet` as its dependency by changing the `module =` line to:
   
   ```
   module = bld.create_ns3_module('simple-application', ['core', 'internet', 'applications'])
   ```

2. Add the following source file as:

   **ns3/contrib/simple-application/model/simple-application.cc**

   ```c++
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
   ```

3. Add the following header file as:

   **ns3/contrib/simple-application/model/simple-application.h**

   ```c++
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
    
    #ifndef SIMPLE_APPLICATION_H
    #define SIMPLE_APPLICATION_H
    
    #include "ns3/application.h"
    #include "ns3/event-id.h"
    #include "ns3/ptr.h"
    #include "ns3/address.h"
    #include "ns3/traced-callback.h"
    #include "ns3/seq-ts-header.h"
    #include "ns3/inet-socket-address.h"
    
    namespace ns3 {
    
        class Socket;
        class Packet;
    
        class SimpleApplication : public Application
        {
        public:
            static TypeId GetTypeId (void);
            SimpleApplication ();
            virtual ~SimpleApplication ();
    
        protected:
            virtual void DoDispose (void);
    
        private:
            virtual void StartApplication (void);
            virtual void StopApplication (void);
            void HandleRead (Ptr<Socket> socket);
    
            uint16_t m_port;      //!< Port on which we listen for incoming packets.
            Ptr<Socket> m_socket; //!< IPv4 Socket
            Address m_local;      //!< Local address (IP, port)
            Address m_peerAddress; //!< Remote peer address (IP, port)
    
        };
    
    } // namespace ns3
    
    #endif /* SIMPLE_APPLICATION_H */
   ```
   
4. Add the following source file as:

   **ns3/contrib/simple-application/helper/simple-application-helper.cc**

   ```c++
   /* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
   /*
    * Copyright (c) 2008 INRIA
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
    * Adapted from UdpEchoHelper by:
    * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
    */
   
   #include "simple-application-helper.h"
   
   namespace ns3 {
   
       SimpleApplicationHelper::SimpleApplicationHelper (uint16_t port, Address address)
       {
           m_factory.SetTypeId (SimpleApplication::GetTypeId ());
           SetAttribute ("RemoteAddress", AddressValue (address));
           SetAttribute ("Port", UintegerValue (port));
       }
   
       void
       SimpleApplicationHelper::SetAttribute (std::string name, const AttributeValue &value) {
           m_factory.Set (name, value);
       }
   
       ApplicationContainer
       SimpleApplicationHelper::Install (Ptr<Node> node) const
       {
           return ApplicationContainer (InstallPriv (node));
       }
   
       ApplicationContainer
       SimpleApplicationHelper::Install (NodeContainer c) const
       {
           ApplicationContainer apps;
           for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i) {
               apps.Add (InstallPriv (*i));
           }
           return apps;
       }
   
       Ptr<Application>
       SimpleApplicationHelper::InstallPriv (Ptr<Node> node) const
       {
           Ptr<Application> app = m_factory.Create<SimpleApplication> ();
           node->AddApplication (app);
           return app;
       }
   
   } // namespace ns3
   ```
   
5. Add the following source file as

   **ns3/contrib/simple-application/helper/simple-application-helper.h**

   ```c++
   /* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
   /*
    * Copyright (c) 2008 INRIA
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
    * Adapted from UdpEchoHelper by:
    * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
    */
   
   #ifndef SIMPLE_APPLICATION_HELPER_H
   #define SIMPLE_APPLICATION_HELPER_H
   
   #include <stdint.h>
   #include "ns3/application-container.h"
   #include "ns3/node-container.h"
   #include "ns3/object-factory.h"
   #include "ns3/ipv4-address.h"
   #include "ns3/ipv6-address.h"
   #include "ns3/simple-application.h"
   #include "ns3/uinteger.h"
   #include "ns3/names.h"
   
   namespace ns3 {
   
       class SimpleApplicationHelper
       {
       public:
           SimpleApplicationHelper (uint16_t port, Address address);
           void SetAttribute (std::string name, const AttributeValue &value);
           ApplicationContainer Install (Ptr<Node> node) const;
           ApplicationContainer Install (NodeContainer c) const;
   
       private:
           Ptr<Application> InstallPriv (Ptr<Node> node) const;
           ObjectFactory m_factory;
       };
       
   
   } // namespace ns3
   
   #endif /* SIMPLE_APPLICATION_HELPER_H */
   ```
   
6.  **Reconfigure and rebuild**

    ```
    ./waf configure --build-profile=debug --enable-mpi --enable-examples --enable-tests --enable-gcov --out=build/debug_all
    ./waf
    ```


## Next chapter

[&#187; Continue to chapter 7: Writing a manual script](7_ns3_script_manually.md)
