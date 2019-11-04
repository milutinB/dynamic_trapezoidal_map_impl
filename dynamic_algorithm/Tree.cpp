#pragma once

#include "Tree.h"


bool complete_bounding_box(BoundingBox* bounding_box) 
{
	bool result = bounding_box->bottom != nullptr;

	result &= bounding_box->right != nullptr;

	result &= bounding_box->top != nullptr;

	result &= bounding_box->left != nullptr;

	return result;
}

void climb_tree(Node* parent, Node* child, BoundingBox* bounding_box) 
{


	if (complete_bounding_box(bounding_box) || parent == nullptr) 
	{
		return;
	}

	if (child == parent->negative_child) 
	{
		//cout << "\negative child\n";
		if (parent->cut.type == SEGMENT) 
		{
			if (bounding_box->top == nullptr) 
			{
				bounding_box->top = &parent->cut;
			}
			climb_tree(parent->parent, parent, bounding_box);
			return;
		}

		//cout << "...\n";

		//cout << parent->cut;

		//cout << "...\n";

		if (bounding_box->right == nullptr) 
		{
			bounding_box->right = &parent->cut;
		}
		climb_tree(parent->parent, parent, bounding_box);
		return;

	}
	else if (child == parent->positive_child)
	{
		//cout << "\npositive child\n";
		if (parent->cut.type == SEGMENT)
		{
			if (bounding_box->bottom == nullptr)
			{
				bounding_box->bottom = &parent->cut;
			}
			climb_tree(parent->parent, parent, bounding_box);
			return;
		}

		if (bounding_box->left == nullptr) 
		{
			bounding_box->left = &parent->cut;
		}
		climb_tree(parent->parent, parent, bounding_box);
		return;

	}

	cout << "\n\nERROR\n\n";
}

TrapezoidType get_bounding_box_type(BoundingBox* bounding_box) 
{

	Line_2 bottom_line = bounding_box->bottom->get_defining_line();
	Line_2 right_line = bounding_box->right->get_defining_line();
	Line_2 top_line = bounding_box->top->get_defining_line();
	Line_2 left_line = bounding_box->left->get_defining_line();

	if (CGAL::parallel(bottom_line, top_line)) 
	{
		return BRTL;
	}

	Point_2 bottom_top_intersection = intersect_lines(bottom_line, top_line);
	Point_2 bottom_right_intersection = intersect_lines(bottom_line, right_line);
	Point_2 bottom_left_intersection = intersect_lines(bottom_line, left_line);

	if (!Line_2(bottom_top_intersection, Vector_2(0, -1)).has_on_negative_side(bottom_right_intersection) && !Line_2(bottom_top_intersection, Vector_2(0, -1)).has_on_positive_side(bottom_left_intersection))
	{
		return BTL;
	}

	if (!Line_2(bottom_top_intersection, Vector_2(0, -1)).has_on_positive_side(bottom_left_intersection) && !Line_2(bottom_top_intersection, Vector_2(0, -1)).has_on_negative_side(bottom_right_intersection))
	{
		return BRT;
	}


	return BRTL;
}

BoundingBox Tree::get_bounding_box(Node* node) 
{
	BoundingBox bounding_box = BoundingBox();

	climb_tree(node->parent, node, &bounding_box);

	if (bounding_box.bottom == nullptr) 
	{
		bounding_box.bottom = enclosing_box.bottom;
	}

	if (bounding_box.right == nullptr) 
	{
		bounding_box.right = enclosing_box.right;
	}

	if (bounding_box.top == nullptr) 
	{
		bounding_box.top = enclosing_box.top;
	}

	if (bounding_box.left == nullptr) 
	{
		bounding_box.left = enclosing_box.left;
	}

	bounding_box.type = get_bounding_box_type(&bounding_box);

	return bounding_box;
}

Node* find_deepest_node_covering_segment(Segment* segment, Node* root) 
{
	return new Node();
}

bool Tree::region_contains_point(Node* node, Point_2 query_point) 
{
	BoundingBox bounding_box = get_bounding_box(node);

	return bounding_box.contains_point(query_point);
}

bool Tree::region_contains_point_strict(Node* node, Point_2 query_point) 
{
	BoundingBox bounding_box = get_bounding_box(node);

	return bounding_box.contains_point_strict(query_point);
}


bool Tree::segment_intersects_region(Node* node, Segment* segment) 
{
	BoundingBox bounding_box = get_bounding_box(node);


	bool region_strictly_contains_source = bounding_box.contains_point_strict(segment->seg.source());
	bool region_strictly_contains_target = bounding_box.contains_point_strict(segment->seg.target());

	// Handles the case where the region contains an endpoint of the segment
	if (region_strictly_contains_source || region_strictly_contains_target) 
	{
		return true;
	}

	vector<Segment_2> segs = bounding_box.cuts_to_segments();
	vector<Segment_2> top_and_bottom = bounding_box.get_top_and_bottom_segments();

	// Handles the case where the new segment cuts the top and bottom
	if (check_for_segment_intersection(top_and_bottom[0], segment->seg) && check_for_segment_intersection(top_and_bottom[1], segment->seg)) 
	{
		return true;
	}

	vector<Point_2> intersection_points = vector<Point_2>();

	for (auto s : segs) 
	{
		if (check_for_segment_intersection(s, segment->seg)) 
		{
			Point_2 p = intersect_segments(s, segment->seg);
			if (intersection_points.size() > 0) 
			{
				if (intersection_points[intersection_points.size() - 1] != p) 
				{
					intersection_points.push_back(p);
				}
			}
			else 
			{
				intersection_points.push_back(p);
			}
		}
	}


	return intersection_points.size() > 1;
}

/*
bool Tree::segment_intersects_region(Node* node, Segment* segment) 
{
	BoundingBox bounding_box = get_bounding_box(node);
	//cout << bounding_box;
	vector<Segment_2> segs = bounding_box.cuts_to_segments();

	for (auto s : segs) 
	{
		cout << s;
		cout << "\n";
	}

	int intersection_count = 0;

	for (auto s : segs) 
	{
		if (check_for_proper_intersection(s, segment->seg)) 
		{
			intersection_count++;
		}
	}

	bool region_strictly_contains_source = bounding_box.contains_point_strict(segment->seg.source());
	bool region_strictly_contains_target = bounding_box.contains_point_strict(segment->seg.target());

	return intersection_count > 1 || region_strictly_contains_source || region_strictly_contains_target;
} */

void Tree::v_partition(Node* node, Cut& vertical_cut) 
{
	node->cut = vertical_cut;

	node->negative_child = new Node();
	node->negative_child->parent = node;

	node->positive_child = new Node();
	node->positive_child->parent = node;
}

void Tree::partition(Node* node, Segment* segment) 
{
	node->cut = Cut();
	node->cut.type = SEGMENT;
	node->cut.segment = segment;

	node->negative_child = new Node();
	node->negative_child->parent = node;

	node->positive_child = new Node();
	node->positive_child->parent = node;
}

vector<Node*> Tree::search(Segment* segment) 
{
	vector<Node*> leaves = vector<Node*>();
	for (auto node : nodes) 
	{
		if (node->is_leaf() && segment_intersects_region(node, segment)) 
		{
			leaves.push_back(node);
		}
	}

	return leaves;
}

CutType Tree::determine_vertical_cut_type(Node* node, Segment* segment) 
{

	//cout << "\nhello from determine\n";
	if (region_contains_point_strict(node, segment->seg.source())) 
	{
		return VERTICAL_SOURCE;
	}

	BoundingBox bounding_box = get_bounding_box(node);



	/*if (bounding_box.left->segment->seg == segment->seg || bounding_box.right->segment->seg == segment->seg) 
	{
		return NONE;
	}*/
	
	vector<Segment_2> top_and_bottom_segments = bounding_box.get_top_and_bottom_segments();

	cout << top_and_bottom_segments[0] << "\n";
	cout << top_and_bottom_segments[1] << "\n";

	if (check_for_proper_intersection(top_and_bottom_segments[0], segment->seg) || check_for_proper_intersection(top_and_bottom_segments[1], segment->seg)) 
	{
		return SEGMENT_INTERSECTION;
	}

	if (region_contains_point_strict(node, segment->seg.target())) 
	{
		//cout << get_bounding_box(node);
		return VERTICAL_TARGET;
	}

	return NONE;
	
}

void Tree::insert(Segment* segment) 
{
	vector<Node*> leaves = search(segment);
	cout << "search complete\n";
	cout << "found " << leaves.size() << " leaves\n";
	vector<Node*> nodes_to_be_partitioned = vector<Node*>();


	while (!leaves.empty()) 
	{
		cout << "hello from insert\n";
		cout << leaves.size() << "\n";
		//cout << segment->seg;
		Node* leaf = leaves[leaves.size()-1];
		leaves.pop_back();

		CutType type = determine_vertical_cut_type(leaf, segment);

		if (type == VERTICAL_SOURCE) 
		{
			cout << "VERTICAL SOURCE\n";
			leaf->cut = Cut();
			leaf->cut.type = VERTICAL_SOURCE;
			leaf->cut.segment = segment;
		}

		if (type == VERTICAL_TARGET) 
		{
			cout << "VERTICAL TARGET\n";
			leaf->cut = Cut();
			leaf->cut.type = VERTICAL_TARGET;
			leaf->cut.segment = segment;
		}

		if (type == SEGMENT_INTERSECTION) 
		{
			cout << "segment intersection \n";
			//cout << Tree::get_bounding_box(leaf);
			BoundingBox bounding_box = get_bounding_box(leaf);
			vector<Segment_2> bottom_top = bounding_box.get_top_and_bottom_segments();

			leaf->cut = Cut();
			leaf->cut.type = SEGMENT_INTERSECTION;
			leaf->cut.segment = segment;

			if (check_for_proper_intersection(bottom_top[0], segment->seg)) 
			{
				cout << "intersection with bottom\n";
				leaf->cut.intersecting_segment = bounding_box.bottom->segment;
			}
			else 
			{
				cout << "intersection with top\n";
				leaf->cut.intersecting_segment = bounding_box.top->segment;
			}
		}

		if (type == NONE) 
		{
			cout << "NONE \n";
			if (segment_intersects_region(leaf, segment)) 
			{
				cout << "intersects region\n";
				nodes_to_be_partitioned.push_back(leaf);
			}
			else 
			{
				cout << "does not intersect leaf\n";

				vector<Segment_2> segs = get_bounding_box(leaf).cuts_to_segments();

				for (auto s : segs) 
				{
					cout << s << "\n";
				}

			}
		}
		else 
		{
			v_partition(leaf, leaf->cut);
			leaves.push_back(leaf->positive_child);
			leaves.push_back(leaf->negative_child);
			nodes.push_back(leaf->negative_child);
			nodes.push_back(leaf->positive_child);
		}
	}

	for (int i = 0; i < nodes_to_be_partitioned.size(); ++i) 
	{
	
		partition(nodes_to_be_partitioned[i], segment);
		cout << "partition " << i << "\n";
		nodes.push_back(nodes_to_be_partitioned[i]->negative_child);

		cout << get_bounding_box(nodes_to_be_partitioned[i]->negative_child);
		vector<Segment_2> neg_segs =  get_bounding_box(nodes_to_be_partitioned[i]->negative_child).cuts_to_segments();

		for (auto s : neg_segs) 
		{
			cout << s << "\n";
		}
		cout << get_bounding_box(nodes_to_be_partitioned[i]->positive_child);

		vector<Segment_2> pos_segs = get_bounding_box(nodes_to_be_partitioned[i]->positive_child).cuts_to_segments();

		for (auto s : pos_segs)
		{
			cout << s << "\n";
		}

		nodes.push_back(nodes_to_be_partitioned[i]->positive_child);
	}

}


