# Chapter 1: The toolbox

Before we get started with the fundamentals, we quickly go through an overview
of the "toolbox" of a computer network researcher and the place of ns-3 in there.
Ns-3 is a discrete-event network simulator.

1. **Rough calculations analysis**
   - Via (rough) calculations demonstrate the expected benefits of a proposed system.
   - Advantage: it shows intuitively the benefit to the reader
   - Caveat: it is at best merely a well-reasoned indication or hypothesis
   
2. **Mathematical/analytic analysis**
   - Proving that the proposed system under some modeling constraints has a bounded
     performance or upholds some important guarantees.
   - Advantage: there are guarantees of how well the system performs
   - Caveat: these guarantees only hold under the modeling constraints,
     and as such often do not account for all the circumstances in practice (which are not modeled)
   
3. **Experiments**

   The proposed system (as a prototype) is implemented in some fashion. You perform
   experiments and collect statistics to evaluate its performance.
   
   Types:
   * **Production:**
     - The implementation is run on the scale of production
     - Advantage: best evidence to show it works (or not)
     - Caveats: extremely costly and typically you don't have access to this,
       often requires significant implementation effort
   
   * **Testbed:**
     - The implementation is run on a scaled-down version 
       of the deployment setting the system is intended for
       for a simulated workload
     - Advantage: close to the real deployment
     - Caveats: significant implementation effort, hardware might still not be readily available
   
   * **Emulation:**
     - The implementation is run on emulated hardware
     - Advantage: close to the testbed, same code generally, real-time timing issues are exposed
     - Caveats: emulation incurs a significant overhead which limits the scale severely,
       especially when it concerns a network which consists of many computers
     - Example framework: Mininet
   
   * **Discrete-event simulation (often at packet-level granularity):**
     - The system is implemented in a reduced simpler version in the framework of a simulator,
       which runs in simulation time.
     - Advantage: implementation is simpler as it does not have to account for real-time.
       Because it is only a model of the real system, it is possible to selectively model/implement
       certain components and simplify or leave out others
     - Caveats: does not account for real-time, scaling is limited to how well it can be parallelized,
       it is only as good as its model of the physical system
     - Example frameworks: OMNET++, ns-3


## Navigation

**Continue to the next chapter:**

[&#187; Chapter 2: What is discrete-event simulation?](2_discrete_event_simulation.md)

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
