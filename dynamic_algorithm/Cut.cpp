#pragma once

#include "Cut.h"

double Cut::priority() 
{
	return this->segment->priority;
}


Point_2 intersect_lines(Line_2 line_1, Line_2 line_2)
{
	Point_2 intersection_point;

	CGAL::Object result = CGAL::intersection(line_1, line_2);

	if (CGAL::assign(intersection_point, result)) 
	{
		return intersection_point;
	}

	std::cout << "lines don't intersect!\n";

	return Point_2(-10000, -10000);
}

Point_2 intersect_segments(Segment_2 seg_1, Segment_2 seg_2) 
{
	Point_2 intersection_point;

	CGAL::Object result = CGAL::intersection(seg_1, seg_2);

	if (CGAL::assign(intersection_point, result))
	{
		return intersection_point;
	}

	std::cout << "segments don't intersect!\n";

	return Point_2(-10000, -10000);
}

Line_2 Cut::get_defining_line() 
{
	if (this->type == VERTICAL_SOURCE) 
	{
		return Line_2(this->segment->seg.source(), Vector_2(0, -1));
	}

	if (this->type == VERTICAL_TARGET) 
	{
		return Line_2(this->segment->seg.target(), Vector_2(0, -1));
	}

	if (this->type == SEGMENT) 
	{
		return Line_2(this->segment->seg);
	}

	if (this->type == SEGMENT_INTERSECTION) 
	{
		Line_2 line_1 = Line_2(this->segment->seg);
		Line_2 line_2 = Line_2(this->intersecting_segment->seg);

		Point_2 intersection_point;

		CGAL::Object result = CGAL::intersection(line_1, line_2);

		if (CGAL::assign(intersection_point, result)) 
		{
			return Line_2(intersection_point, Vector_2(0, -1));
		}

		std::cout << "error intersecting lines in get_defining_line_method\n";
	}

	std::cout << "cut does not have a tpe set\n";
}

bool Cut::negative_side(Point_2* query_point) 
{
	Line_2 defining_line = get_defining_line();

	return defining_line.has_on_negative_side(*query_point);
}

bool Cut::positive_side(Point_2* query_point) 
{
	Line_2 defining_line = get_defining_line();

	return defining_line.has_on_positive_side(*query_point);
}

bool Cut::on_defining_line(Point_2* query_point) 
{
	Line_2 defining_line = get_defining_line();

	return defining_line.has_on(*query_point);
}

bool check_for_segment_intersection(Segment_2 seg_1, Segment_2 seg_2)
{
	Point_2 intersection_point;
	CGAL::Object result = CGAL::intersection(seg_1, seg_2);

	if (CGAL::assign(intersection_point, result))
	{
		return true;
	}

	return false;
}

bool check_for_proper_intersection(Segment_2 seg_1, Segment_2 seg_2) 
{
	bool intersect = check_for_segment_intersection(seg_1, seg_2);

	return intersect && seg_1.source() != seg_2.source() && seg_1.source() != seg_2.target() && seg_1.target() != seg_2.source() && seg_1.target() != seg_2.target() && !seg_2.has_on(seg_1.source()) && !seg_2.has_on(seg_1.target());
}

ostream& operator<<(ostream& c, Cut& cut) 
{

	if (cut.type == SEGMENT) 
	{
		c << "segment type\n";
	}

	if (cut.type == VERTICAL_SOURCE)
	{
		c << "vertical source type\n";
	}

	if (cut.type == VERTICAL_TARGET)
	{
		c << "vertical target type\n";
	}

	if (cut.type == SEGMENT_INTERSECTION)
	{
		c << "segment intersection type\n";
	}

	c << *cut.segment;
	c << "\n";

	return c;
}