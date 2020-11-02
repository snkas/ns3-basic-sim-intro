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
