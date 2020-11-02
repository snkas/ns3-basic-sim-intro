# Chapter 9: Writing a script with basic-sim

We are now going to write a script which makes use of our newly written 
`simple-application` combined with `basic-sim` to run it as a framework.

1. Make a directory somewhere (let's call it "example_one") which servers as the run directory.
   In this directory all input and output files are (or will be) placed.

2. `basic-sim` reads at the start a run configuration file named `config_ns3.properties`.
   In here you define the simulation end time and the seed, and other properties.
   The syntax and valid properties we will get into later.
   
   Add the following file:

   **/path/to/example_one/config_ns3.properties**
   ```
   simulation_end_time_ns=1000000000
   simulation_seed=123456789
   
   topology_ptop_filename="ptop_topology.properties"
   ```

3. `basic-sim` offers the ability to create a network-topology from a graph-like
   configuration file. The syntax and valid properties we will get into later.

   Add the following file:

   **/path/to/example_one/ptop_topology.properties**
   ```
   # One link topology:
   # 0 -- 1
   
   num_nodes=2
   num_undirected_edges=1
   switches=set(0,1)
   switches_which_are_tors=set(0,1)
   servers=set()
   undirected_edges=set(0-1)
   
   # 2ms delay, 5.0 Mbit/s, 100 packet queue for all links, no error model, no queueing discipline
   link_channel_delay_ns=2000000
   link_net_device_data_rate_megabit_per_s=5.0
   link_net_device_queue=drop_tail(100p)
   link_net_device_receive_error_model=none
   link_interface_traffic_control_qdisc=disabled
   ```
   
4. Just like manual ns-3, you need to write a script (inside your scratch folder) to run it:

   **ns3/scratch/simple-main-basic-sim.cc**
   ```
   #include <ns3/command-line.h>
   #include "ns3/basic-simulation.h"
   #include "ns3/topology-ptop.h"
   #include "ns3/ipv4-arbiter-routing-helper.h"
   #include "ns3/arbiter-ecmp-helper.h"
   #include "ns3/simple-application-helper.h"
   
   using namespace ns3;
   
   int main(int argc, char *argv[]) {
   
       // No buffering of printf
       setbuf(stdout, nullptr);
   
       // Retrieve run directory
       CommandLine cmd;
       std::string run_dir = "";
       cmd.Usage("Usage: ./waf --run=\"my-main --run_dir='<path/to/run/directory>'\"");
       cmd.AddValue("run_dir",  "Run directory", run_dir);
       cmd.Parse(argc, argv);
       if (run_dir.compare("") == 0) {
           printf("Usage: ./waf --run=\"my-main --run_dir='<path/to/run/directory>'\"");
           return 1;
       }
   
       // Load basic simulation environment
       Ptr<BasicSimulation> basicSimulation = CreateObject<BasicSimulation>(run_dir);
   
       // Read point-to-point topology, and install routing arbiters
       Ptr<TopologyPtop> topology = CreateObject<TopologyPtop>(basicSimulation, Ipv4ArbiterRoutingHelper());
       ArbiterEcmpHelper::InstallArbiters(basicSimulation, topology);
   
       // Run the custom application between node 0 and 1
   
       NodeContainer nodes = topology->GetNodes();
   
       SimpleApplicationHelper helper0(1234, InetSocketAddress(nodes.Get(1)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal(), 1234));
       ApplicationContainer app0 = helper0.Install(nodes.Get(0));
       app0.Start(NanoSeconds(0));
   
       SimpleApplicationHelper helper1(1234, helper1(1234, InetSocketAddress(nodes.Get(0)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal(), 1234));
       ApplicationContainer app1 = helper1.Install(nodes.Get(1));
       app1.Start(NanoSeconds(0));
   
       // Run simulation
       basicSimulation->Run();
   
       // Finalize the simulation
       basicSimulation->Finalize();
   
       return 0;
   
   }
   ```
   
5. Now we run the whole ensemble:

   ```
   ./waf --run="scratch/simple-main-basic-sim --run_dir='/path/to/example_one'" 
   ```

6. Which should give as output:

   ```
   (... many other lines ...)
   SIMULATION
   Running the simulation for 1.00 simulation seconds...
   +0.0ns -- Node 0: created UDP socket (IP=0.0.0.0 (which means any local IP address), port=1234) 
   +0.0ns -- Node 0: sent out packet (seq=0 time=0) to 10.0.0.2:1234
   +0.0ns -- Node 1: created UDP socket (IP=0.0.0.0 (which means any local IP address), port=1234) 
   +0.0ns -- Node 1: sent out packet (seq=1 time=0) to 10.0.0.1:1234
   +2067200.0ns -- Node 1: received packet (seq=0 time=0) from 10.0.0.1:1234
   +2067200.0ns -- Node 0: received packet (seq=1 time=0) from 10.0.0.2:1234
   Finished simulation.
   Simulation of 1.0 seconds took in wallclock time 0.0 seconds.
   (... many other lines ...)
   ```


## Do the full basic-sim tutorial
   
Now do the complete `basic-sim` getting-started tutorial which includes already
running TCP flows, UDP bursts and collecting link statistics (e.g., utilization, queue size)!

https://github.com/snkas/basic-sim/blob/master/doc/getting_started.md


## Next chapter

[Continue to chapter 10: Further readings and test](10_further_readings.md)
