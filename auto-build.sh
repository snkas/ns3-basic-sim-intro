# Making the ns-3 folder
echo "Building base ns-3..."
unzip -n ns-3.33.zip || exit 1
mkdir -p ns-3 || exit 1
scp -r ns-3.33/* ns-3/ || exit 1
rm -r ns-3.33 || exit 1
cd ns-3 || exit 1
./waf configure --build-profile=debug --enable-mpi --enable-examples --enable-tests --enable-gcov --out=build/debug_all || exit 1
./waf || exit 1
cd .. || exit 1

# ns-3 tutorial
echo "Running first ns-3 tutorial..."
cd ns-3 || exit 1
cp examples/tutorial/first.cc scratch/myfirst.cc || exit 1
./waf || exit 1
./waf --run scratch/myfirst || exit 1
cd .. || exit 1

# Adding own module
echo "Adding own module..."
cd ns-3 || exit 1
python utils/create-module.py simple-application || exit 1
cp ../.auto-build-files/wscript contrib/simple-application || exit 1
cp ../.auto-build-files/simple-application.cc contrib/simple-application/model || exit 1
cp ../.auto-build-files/simple-application.h contrib/simple-application/model || exit 1
cp ../.auto-build-files/simple-application-helper.cc contrib/simple-application/helper || exit 1
cp ../.auto-build-files/simple-application-helper.h contrib/simple-application/helper || exit 1
./waf configure --build-profile=debug --enable-mpi --enable-examples --enable-tests --enable-gcov --out=build/debug_all || exit 1
./waf || exit 1
cd .. || exit 1

# ns-3 script manually
echo "Running manual script..."
cd ns-3 || exit 1
cp ../.auto-build-files/simple-main-manual.cc scratch/ || exit 1
./waf --run scratch/simple-main-manual || exit 1
cd .. || exit 1

# Installing basic-sim
echo "Installing basic-sim..."
cd ns-3/contrib || exit 1
if [ ! -d "basic-sim" ] ; then
  git clone https://github.com/snkas/basic-sim.git || exit 1
fi
cd .. || exit 1
./waf configure --build-profile=debug --enable-mpi --enable-examples --enable-tests --enable-gcov --out=build/debug_all || exit 1
./waf || exit 1
cd .. || exit 1

# ns-3 script with basic-sim
echo "Running script with basic-sim..."
cd ns-3 || exit 1
mkdir -p example_one || exit 1
cp ../.auto-build-files/config_ns3.properties example_one/ || exit 1
cp ../.auto-build-files/ptop_topology.properties example_one/ || exit 1
cp ../.auto-build-files/simple-main-basic-sim.cc scratch/ || exit 1
./waf --run="scratch/simple-main-basic-sim --run_dir='example_one'" || exit 1
cd .. || exit 1
