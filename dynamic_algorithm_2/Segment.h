#pragma once


#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Direction_2.h>
#include <CGAL/Aff_transformation_2.h>
#include <CGAL/aff_transformation_tags.h>
//#include "Cut.h"


typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Line_2<Kernel> Line_2;
typedef CGAL::Vector_2<Kernel> Vector_2;
typedef CGAL::Direction_2<Kernel> Direction_2;
typedef CGAL::Aff_transformation_2<Kernel> Transformation;

using namespace std;


struct Segment
{
	Segment_2 seg;
	double priority;

	//Cut* vertical_source = nullptr;
	//Cut* vartical_target = nullptr;
	//Cut* segment_cut = nullptr;
};

ostream& operator<<(ostream& c, Segment& segment);