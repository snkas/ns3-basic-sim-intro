# Chapter 5: Building ns-3

Now that you have some understanding of ns-3, we continue with building ns-3.
There are several ways supported, though the primary one also used by the
developer is `waf`. In this introduction, we only focus on the C++ part of
ns-3, as the Python bindings are just a wrapper around that.


## Requirements

1. Although it is not necessary per se, it is recommended to develop on a Linux-based
   operating system. If you don't have that on your laptop / desktop, it is recommended
   to create a VirtualBox with Ubuntu 20. It is a factor slower, but for now the examples
   of this tutorial are not very demanding.

2. Make sure Python 3.7+ is the default `python` (e.g., by installing anaconda)

3. Install MPI and lcov:

   * MPI:
     ```
     sudo apt-get install openmpi-bin openmpi-common openmpi-doc libopenmpi-dev
     ```
   * lcov: `sudo apt-get install lcov`


## Building

1. Go to the `ns3-basic-sim-intro` folder:

   ```
   cd /path/to/ns3-basic-sim-intro
   ```
   
2. Extract ns-3.31.zip into a folder named `ns-3/`:

   ```
   unzip -n ns-3.31.zip
   mv ns-3.31 ns-3
   ```
   
3. Enter the `ns-3` folder:
   ```
   cd ns-3
   ```

4. Configure ns-3 (basically, specify what needs to be build, and what needs to be skipped)
   ```
   ./waf configure --build-profile=debug --enable-mpi --enable-examples --enable-tests --enable-gcov --out=build/debug_all
   ```
   
   Tip: to get better performance, you can set --build-profile=optimized to have the compiler
   remove logging and remove various (generally redundant) checks. It is recommended to develop in debug,
   so for the remainder of this tutorial KEEP DEBUG.
   
5. Build ns-3 by executing `waf`
   ```
   ./waf
   ```
   
   This will take a while. However, if you edit any ns-3 code and execute this again,
   only the delta needs to be compiled.


## What is in the `ns-3/` folder?

ns-3 is in there now, completely built. Now a bit of context for each folder 
so you understand the structure:

The following files and folders in `ns-3/` are interesting for now:

- `waf`

  This is the primary script to build and run.
  
- `test.py`

  This is the primary script to run tests.

- `build/`

  Build output: the compiled source files and the executables they produce.
  
- `src/`

   The ns-3 modules which are included by default in ns-3.
   
- `contrib/`

  You place your own user-defined C++ ns-3 modules here! Typically if you find someone with a
  contribution to ns-3, they will provide an ns-3 module folder which you can place here.
  It is like an addition next to the modules in `src/`.
  
  (Note: ns-3 is designed to be modular. Of course, this is not always perfect:
  sometimes some ns-3 code you find online needs edits in the src/ code to work.
  This gets messy quickly. In general, you should STRONGLY AVOID 
  changing code in `src/` if at all possible, or separate the changes and re-apply
  them at each compilation.)
  
- `scratch/`

  This is similar to `contrib/` except that this is for simple C++ scripts you write,
  and not as much for real modules which extend ns-3. You can create a "module" here
  by creating a folder and placing your source files in there, however, they cannot
  be imported outside of that folder and are thus limited to one main script. 
  
- `examples/`

  ns-3 examples used in their documentation. In the next chapter you will actually make use
  of the `examples/tutorial/first.cc`.

The following folders in `simulator/` are (for now) not interesting:

- `doc/` -- Documentation building.
- `bindings/` -- Python bindings.
- `utils/` -- Some common command scripts the ns-3 developers found handy 
  (like code coverage and rescaling pdfs).
- `waf-tools/` -- Build tools for the ns-3 code.
- (several other files) 


## Navigation

**Continue to the next chapter:**

[&#187; Chapter 6: First ns-3 tutorial](6_ns3_tutorial.md)

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
