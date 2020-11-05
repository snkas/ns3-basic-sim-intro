## Chapter 9: Introducing basic-sim

As you might have noticed in Chapter 7, you have to write a lot of boiler-plate code
to make an experimental setup. For example, if you are just writing an application,
you generally do not have very specific requirements for the network topology.

The `basic-sim` ns-3 module is intended to make this process a bit easier.
It has a wrapper to take care of loading in run folder configurations (e.g., runtime, random seed),
a topology abstraction, an additional routing abstraction called "arbiter routing",
a heuristic TCP optimizer, and a few handy applications (TCP flows, UDP bursts, and UDP pings).

1. **Adding the module to your ns-3 modules**

   Follow the instructions at:

   https://github.com/snkas/basic-sim/README.md
   
   In order to add the `basic-sim` ns-3 module to your build of ns-3.
   
2. **Configure the build**

   Newly copied modules in `src/` and `contrib/` must be first added to the build,
   so you need to configure again
   ```
   ./waf configure --build-profile=debug --enable-mpi --enable-examples --enable-tests --enable-gcov --out=build/debug_all
   ```
   
3. **Rebuild**

   ```
   ./waf
   ```
   
## Navigation

**Continue to the next chapter:**

[&#187; Chapter 10: Writing a script with basic-sim](10_ns3_script_with_basic_sim.md)

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
