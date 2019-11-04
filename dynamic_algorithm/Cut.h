#pragma once
#include <iostream>
#include "Segment.h"

enum CutType { VERTICAL_SOURCE, VERTICAL_TARGET,  SEGMENT, SEGMENT_INTERSECTION, NONE };

using namespace std;

struct Cut 
{
	Segment* segment;

	Segment* intersecting_segment;

	CutType type;

	double priority();

	Line_2 get_defining_line();

	bool negative_side(Point_2* query_point);

	bool positive_side(Point_2* query_point);

	bool on_defining_line(Point_2* query_point);
};

Point_2 intersect_lines(Line_2 line_1, Line_2 line_2);

Point_2 intersect_segments(Segment_2 seg_1, Segment_2 seg_2);

bool check_for_segment_intersection(Segment_2 seg_1, Segment_2 seg_2);

bool check_for_proper_intersection(Segment_2 seg_1, Segment_2 seg_2);

ostream& operator<<(ostream& c, Cut& cut);