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

    SimpleApplicationHelper helper1(1234, InetSocketAddress(nodes.Get(0)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal(), 1234));
    ApplicationContainer app1 = helper1.Install(nodes.Get(1));
    app1.Start(NanoSeconds(0));

    // Run simulation
    basicSimulation->Run();

    // Finalize the simulation
    basicSimulation->Finalize();

    return 0;

}
