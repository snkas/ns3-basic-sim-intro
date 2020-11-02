## Chapter 8: Introducing basic-sim

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
   
## Next chapter

[Continue to chapter 9: Writing a script with basic-sim](9_ns3_script_with_basic_sim.md)
