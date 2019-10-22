#include <algorithm>
#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h> 
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/intersections.h>
#include <ostream>
#include <fstream>


typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Point_3<Kernel> Point_3;
typedef CGAL::Segment_3<Kernel> Segment_3;
using namespace std;

struct Rectangle
{
	double c_x;
	double c_y;
	double c_z;
	double w;
	double l;
	double d_y;
	double d_z;
};
// creates a vector of random rectangle structs
vector<Rectangle> generate_rectangles(int number_of_rectangles);