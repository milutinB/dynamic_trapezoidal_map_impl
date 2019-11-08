#pragma once

#include "Cut.h"
#include <CGAL/Polygon_2.h>

typedef CGAL::Polygon_2<Kernel> Polygon_2;

enum TrapezoidType { BRTL, BTL, BRT };

struct BoundingBox 
{
	TrapezoidType type;

	Cut* bottom = nullptr;
	Cut* right = nullptr;
	Cut* top = nullptr;
	Cut* left = nullptr;

	vector<Segment_2> cuts_to_segments();

	vector<Segment_2> get_top_and_bottom_segments();

	bool contains_point(Point_2 query_point);
	bool contains_point_strict(Point_2 query_point);
	bool endpoint_on_segment(Point_2 query_point);
};

ostream& operator<<(ostream& c, BoundingBox& bounding_box);