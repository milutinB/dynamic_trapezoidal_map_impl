#pragma once

#include "BoundingBox.h"

vector<Segment_2> BoundingBox::cuts_to_segments() 
{
	vector<Segment_2> output;

	Line_2 bottom_line = this->bottom->get_defining_line();
	Line_2 top_line = this->top->get_defining_line();

	if (this->type == BRT) 
	{
		cout << "BRT\n";
		Line_2 right_line = this->right->get_defining_line();
		Point_2 p1 = intersect_lines(bottom_line, right_line);
		Point_2 p2 = intersect_lines(right_line, top_line);
		Point_2 p3 = intersect_lines(top_line, bottom_line);

		output.push_back(Segment_2(p1, p2));
		output.push_back(Segment_2(p2, p3));
		output.push_back(Segment_2(p3, p1));
	}
	else if (this->type == BTL) 
	{
		cout << "BTL\n";
		Line_2 left_line = this->left->get_defining_line();
		Point_2 p1 = intersect_lines(bottom_line, top_line);
		Point_2 p2 = intersect_lines(top_line, left_line);
		Point_2 p3 = intersect_lines(left_line, bottom_line);

		output.push_back(Segment_2(p1, p2));
		output.push_back(Segment_2(p2, p3));
		output.push_back(Segment_2(p3, p1));
	}
	else 
	{
		cout << "BRTL\n";
		Line_2 right_line = this->right->get_defining_line();
		Line_2 left_line = this->left->get_defining_line();
		Point_2 p1 = intersect_lines(bottom_line, right_line);
		Point_2 p2 = intersect_lines(right_line, top_line);
		Point_2 p3 = intersect_lines(top_line, left_line);
		Point_2 p4 = intersect_lines(left_line, bottom_line);


		output.push_back(Segment_2(p1, p2));
		output.push_back(Segment_2(p2, p3));
		output.push_back(Segment_2(p3, p4));
		output.push_back(Segment_2(p4, p1));
	}

	return output;
}

vector<Segment_2> BoundingBox::get_top_and_bottom_segments() 
{
	vector<Segment_2> segs = cuts_to_segments();
	vector<Segment_2> output = vector<Segment_2>();

	if (this->type == BTL ||  this->type == BRT) 
	{
		output.push_back(segs[2]);
		output.push_back(segs[1]);
	}
	else 
	{
		output.push_back(segs[3]);
		output.push_back(segs[1]);
	}

	return output;

}

bool BoundingBox::contains_point(Point_2 query_point) 
{
	Line_2 bottom_line = this->bottom->get_defining_line();
	Line_2 top_line = this->top->get_defining_line();

	bool result = !bottom_line.has_on_negative_side(query_point);
	result &= !top_line.has_on_positive_side(query_point);

	if (this->type == BTL) 
	{
		Line_2 left_line = this->left->get_defining_line();
		result &= !left_line.has_on_negative_side(query_point);
		return result;
	}

	if (this->type == BRT) 
	{
		Line_2 right_line = this->right->get_defining_line();
		result &= !right_line.has_on_positive_side(query_point);
		return result;
	}

	Line_2 left_line = this->left->get_defining_line();
	result &= !left_line.has_on_negative_side(query_point);
	Line_2 right_line = this->right->get_defining_line();
	result &= !right_line.has_on_positive_side(query_point);
	return result;
}

bool BoundingBox::contains_point_strict(Point_2 query_point) 
{
	Line_2 bottom_line = this->bottom->get_defining_line();
	Line_2 top_line = this->top->get_defining_line();

	bool result = bottom_line.has_on_positive_side(query_point);
	result &= top_line.has_on_negative_side(query_point);

	if (this->type == BTL)
	{
		Line_2 left_line = this->left->get_defining_line();
		result &= left_line.has_on_positive_side(query_point);
		return result;
	}

	if (this->type == BRT)
	{
		Line_2 right_line = this->right->get_defining_line();
		result &= right_line.has_on_negative_side(query_point);
		return result;
	}

	Line_2 left_line = this->left->get_defining_line();
	result &= left_line.has_on_positive_side(query_point);
	Line_2 right_line = this->right->get_defining_line();
	result &= right_line.has_on_negative_side(query_point);
	return result;
}

bool BoundingBox::endpoint_on_segment(Point_2 query_point) 
{

	vector<Segment_2> segs = cuts_to_segments();

	if (this->type == BRTL) 
	{
		
	}
	else if (this->type == BRT) 
	{
		
	}
	else 
	{
		
	}

	return false;
}

ostream& operator<<(ostream& c, BoundingBox& bounding_box) 
{

	c << bounding_box.type;
	c << "\n";

	if (bounding_box.top != nullptr)
	{
		c << "top: \n";
		c << *bounding_box.top;
	}

	if (bounding_box.bottom != nullptr)
	{
		c << "bottom: \n";
		c << *bounding_box.bottom;
	}

	if (bounding_box.left != nullptr)
	{
		c << "left: \n";
		c << *bounding_box.left;
	}

	if (bounding_box.right != nullptr)
	{
		c << "right: \n";
		c << *bounding_box.right;
	}


	return c;
}