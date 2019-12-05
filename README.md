## Overview
This repository contains the source code for the experiments reported on in (LINK TO PAPAER). The experiments evaluate a dynamic algorithm for construction of a trapezoidal search tree for a set of 2D line segments with priorities. The experiments run the algorithm when the segments are inserting in increasing order of priority and in random order. After each insertion, the following information is recorded:

* Number of segments already inserted into the tree
* The priority of the last segment inserted
* The number of affected nodes found (See paper for definition)
* The size of the tree after the last insertion
* The depth of the tree after the last insertion
* Number of nodes visited during the search
* Number of calls to the v_partition subroutine
* Number of calls to the partition subroutine
* Number of segments found to intersect the new segment
* Total number of intersections in the current tree
* Execution time of the insert call

Once the experiments are complete, two files, dynamic_insert_stats.dat and static_insert_stats.dat, are created in the directory containing the executable.


## Prerequisites

To compile the code, the following libraries must be installed:

* CGAL 4.14.1 https://www.cgal.org/
* Boost 1.71 https://www.boost.org/

## How to Build
cd /path/to/your/program
cgal_create_CMakeLists -s your_program
mkdir build
cd build
cmake -DCGAL_DIR=XXXXXXXX -DCMAKE_BUILD_TYPE=Debug ../
make

## Running the Executable
The executable takes 4 parameters: the number of segments to be generated, a string indicating the type of experiment, a seed for the random number generator and the step size.

The type can take one of three values:

* 'none' - Horizontal segments are generated resulting in no intersections
* 'few' - Short segments are generated resulting in few intersections
* 'many' - Long segments are generated resulting in many intersections

The step size is the frequency at which the statistics are recomputed. E.g. a step size of 10 will result in a row being printed in the .dat files for every 10 insertions.

Our experiments of 10k non-intersecting segments were run with a step size of 1 with the seed 1337. To reproduce these results, run
```
main.exe 10000 none 1337 1
```

Our experiments for 10k segments with few segments use the seed 1337 and a step size of 10.

```
main.exe 10000 few 1337 10
```
Our experments for 1k segments with many intersections use the seed 1337 and a step size of 1.

```
main.exe 1000 many 1337 1
```
