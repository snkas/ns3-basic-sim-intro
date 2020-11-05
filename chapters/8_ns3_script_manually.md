# Chapter 8: Writing a manual script

We are going to replace the application in `myfirst.cc` of the
ns-3 tutorial with our own application.

1. Add the following source file as:

   **ns3/scratch/simple-main-manual.cc**
   ```
   /* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
   /*
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
    */
   
   #include "ns3/core-module.h"
   #include "ns3/network-module.h"
   #include "ns3/internet-module.h"
   #include "ns3/point-to-point-module.h"
   #include "ns3/applications-module.h"
   #include "ns3/simple-application-helper.h"
   
   // Default Network Topology
   //
   //       10.1.1.0
   // n0 -------------- n1
   //    point-to-point
   //
    
   using namespace ns3;
   
   NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");
   
   int
   main (int argc, char *argv[])
   {
     CommandLine cmd (__FILE__);
     cmd.Parse (argc, argv);
     
     Time::SetResolution (Time::NS);
   
     NodeContainer nodes;
     nodes.Create (2);
   
     PointToPointHelper pointToPoint;
     pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
     pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
   
     NetDeviceContainer devices;
     devices = pointToPoint.Install (nodes);
   
     InternetStackHelper stack;
     stack.Install (nodes);
   
     Ipv4AddressHelper address;
     address.SetBase ("10.1.1.0", "255.255.255.0");
   
     Ipv4InterfaceContainer interfaces = address.Assign (devices);
   
     SimpleApplicationHelper helper0(1234, InetSocketAddress(Ipv4Address("10.1.1.2"), 1234));
     ApplicationContainer app0 = helper0.Install(nodes.Get(0));
     app0.Start(NanoSeconds(0));

     SimpleApplicationHelper helper1(1234, InetSocketAddress(Ipv4Address("10.1.1.1"), 1234));
     ApplicationContainer app1 = helper1.Install(nodes.Get(1));
     app1.Start(NanoSeconds(0));
   
     ns3::RngSeedManager::SetSeed(123456789);
     Simulator::Stop(NanoSeconds(1000000000));
     Simulator::Run ();
     Simulator::Destroy ();
     return 0;
   }
   ```
   
2. Now we run the whole ensemble:

   ```
   ./waf --run scratch/simple-main-manual
   ```

3. Which should give as output:

   ```
   +0.0ns -- Node 0: created UDP socket (IP=0.0.0.0 (which means any local IP address), port=1234) 
   +0.0ns -- Node 0: sent out packet (seq=0 time=0) to 10.1.1.2:1234
   +0.0ns -- Node 1: created UDP socket (IP=0.0.0.0 (which means any local IP address), port=1234) 
   +0.0ns -- Node 1: sent out packet (seq=1 time=0) to 10.1.1.1:1234
   +2067200.0ns -- Node 1: received packet (seq=0 time=0) from 10.1.1.1:1234
   +2067200.0ns -- Node 0: received packet (seq=1 time=0) from 10.1.1.2:1234
   ```
   

## Navigation

**Continue to the next chapter:**

[&#187; Chapter 9: Introducing basic-sim](9_ns3_introducing_basic_sim.md)

**Full menu:**

* [Chapter 1: The toolbox](1_toolbox.md)
* [Chapter 2: What is discrete-event simulation?](2_discrete_event_simulation.md)
* [Chapter 3: What are we going to simulate?](3_what_to_simulate.md)
* [Chapter 4: Ns-3 basics](4_ns3_basics.md)
* [Chapter 5: Building ns-3](5_ns3_building.md)
* [Chapter 6: First ns-3 tutorial](6_ns3_tutorial.md)
* [Chapter 7: Adding your own ns-3 module](7_ns3_adding_your_own_module.md)
* [Chapter 8: Writing a manual script](8_ns3_script_manually.md)
* [Chapter 9: Introducing basic-sim](9_ns3_introducing_basic_sim.md)
* [Chapter 10: Writing a script with basic-sim](10_ns3_script_with_basic_sim.md)
* [Chapter 11: Further readings](11_further_readings.md)
* [Chapter 12: Test your knowledge](12_test_your_knowledge.md)
