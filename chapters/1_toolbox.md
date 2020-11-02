# Chapter 1: The toolbox

Before we get started with the fundamentals, we quickly go through an overview
of the "toolbox" of a computer network researcher and the place of ns-3 in there.
Ns-3 is a discrete-event network simulator.

1. **Rough calculations analysis**
   - Via (rough) calculations demonstrate the expected benefits of a proposed system.
   - Advantage: it shows intuitively the benefit to the reader
   - Disadvantage: it is at best merely a well-reasoned indication or hypothesis
   
2. **Mathematical/analytic analysis**
   - Proving that the proposed system under some constraints has a bounded
     performance or upholds some important guarantees.
   - Advantage: there are guarantees of how well the system performs
   - Disadvantage: these guarantees only hold under the precisely defined constraints,
     and as such do not sufficiently account for "reality"
   
3. **Experiments**

   The proposed system (as a prototype) is implemented in some fashion. You perform
   experiments and collect statistics to evaluate its performance.
   
   Types:
   * **Production:**
     - The implementation is run on the scale of production
     - Advantage: best evidence to show it works (or not)
     - Disadvantage: extremely costly and typically you don't have access to this,
       often requires significant implementation effort
   
   * **Testbed:**
     - The implementation is run on a scaled-down version 
       of the deployment setting the system is intended for
       for a simulated workload
     - Advantage: close to the real deployment
     - Disadvantage: significant implementation effort, hardware might still not be readily available
   
   * **Emulation:**
     - The implementation is run on emulated hardware
     - Advantage: close to the testbed, same code generally, real-time timing issues are exposed
     - Disadvantage: emulation incurs a significant overhead which limits the scale severely,
       especially when it concerns a network which consists of many computers
     - Example framework: Mininet
   
   * **Discrete-event simulation (often at packet-level granularity):**
     - The system is implemented in a reduced simpler version in the framework of a simulator,
       which then runs in simulation time.
     - Advantage: implementation is simpler as it does not have to account for real-time.
       Because it is only a model of the real system, it is possible to selectively model/implement
       certain components and simplify or leave out others
     - Disadvantage: does not account for real-time, scaling is limited to how well it can be parallelized,
       it is only a model of the real thing
     - Example frameworks: OMNET++, ns-3


## Next chapter

[&#187; Continue to chapter 2: What is discrete-event simulation?](2_discrete_event_simulation.md)
