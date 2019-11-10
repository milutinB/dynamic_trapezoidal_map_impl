cd /path/to/your/program
cgal_create_CMakeLists -s your_program
mkdir build
cd build
cmake -DCGAL_DIR=XXXXXXXX -DCMAKE_BUILD_TYPE=Debug ../
make
