#pragma once
#include <fstream>
#include "Tree.h"
#include <CGAL/Direction_2.h>

#define CHECKING 0
#define EXPERIMENTS 1
//#define STEP_SIZE 1

typedef CGAL::Direction_2<Kernel> Direction_2;

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
		if (parent->cut->type == SEGMENT)
		{
			if (bounding_box->top == nullptr)
			{
				bounding_box->top = parent->cut;
			}
			climb_tree(parent->parent, parent, bounding_box);
			return;
		}

		//cout << "...\n";

		//cout << parent->cut;

		//cout << "...\n";

		if (bounding_box->right == nullptr)
		{
			bounding_box->right = parent->cut;
		}
		climb_tree(parent->parent, parent, bounding_box);
		return;

	}
	else if (child == parent->positive_child)
	{
		//cout << "\npositive child\n";
		if (parent->cut->type == SEGMENT)
		{
			if (bounding_box->bottom == nullptr)
			{
				bounding_box->bottom = parent->cut;
			}
			climb_tree(parent->parent, parent, bounding_box);
			return;
		}

		if (bounding_box->left == nullptr)
		{
			bounding_box->left = parent->cut;
		}
		climb_tree(parent->parent, parent, bounding_box);
		return;

	}

	cout << "\n\nERROR\n\n";
}

CGAL::Lazy_exact_nt<CGAL::Gmpq> slope_of_line(Line_2 line)
{
	Direction_2 direction = line.direction();
	return direction.dy() / direction.dx();
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
	Point_2 top_right_intersection = intersect_lines(top_line, right_line);


	if (slope_of_line(top_line) < slope_of_line(bottom_line))
	{
		if (!Line_2(bottom_top_intersection, Vector_2(0, -1)).has_on_negative_side(bottom_right_intersection))
		{
			return BTL;
		}
		else
		{
			return BRTL;
		}
	}

	if (slope_of_line(top_line) > slope_of_line(bottom_line))
	{
		if (!Line_2(bottom_top_intersection, Vector_2(0, -1)).has_on_positive_side(bottom_left_intersection))
		{
			return BRT;
		}
		else
		{
			return BRTL;
		}
	}


	/*
	if (bottom_left_intersection == bottom_top_intersection)
	{
		return BTL;
	}

	if (!Line_2(bottom_top_intersection, Vector_2(0, -1)).has_on_positive_side(bottom_left_intersection) && !Line_2(bottom_top_intersection, Vector_2(0, -1)).has_on_negative_side(bottom_right_intersection))
	{
		return BRT;
	}

	if (!Line_2(bottom_top_intersection, Vector_2(0, -1)).has_on_negative_side(bottom_right_intersection) && !Line_2(bottom_top_intersection, Vector_2(0, -1)).has_on_positive_side(bottom_left_intersection))
	{
		return BTL;
	}


	return BRTL;*/
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

void Tree::get_nodes_from_traversal(Node* node, vector<Node*> &nodes) 
{
	nodes.push_back(node);

	if (!node->is_leaf()) 
	{
		get_nodes_from_traversal(node->positive_child, nodes);
		get_nodes_from_traversal(node->negative_child, nodes);
	}

	return;
}

int Tree::depth(Node* node) 
{
	if (node == nullptr) 
	{
		return 0;
	}

	return max(depth(node->negative_child), depth(node->positive_child)) + 1;
}

int Tree::size(Node* node) 
{
	if (node->is_leaf()) 
	{
		return 0;
	}
	else 
	{
		return 1 + size(node->positive_child) + size(node->negative_child);
	}
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

void Tree::v_partition_naive(Node* node, Cut* vertical_cut)
{
	if (EXPERIMENTS == 1) 
	{
		v_partition_calls++;
	}
	
	node->cut = vertical_cut;

	node->negative_child = new Node();
	node->negative_child->parent = node;

	node->positive_child = new Node();
	node->positive_child->parent = node;
}

void Tree::partition_naive(Node* node, Segment* segment)
{
	if (EXPERIMENTS == 1) 
	{
		partition_calls++;
	}

	node->cut = new Cut();
	node->cut->type = SEGMENT;
	node->cut->segment = segment;

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

	vector<Segment_2> top_and_bottom_segments = bounding_box.get_top_and_bottom_segments();

	//cout << top_and_bottom_segments[0] << "\n";
	//cout << top_and_bottom_segments[1] << "\n";

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

vector<Node*> Tree::get_leaves()
{
	vector<Node*> leaves = vector<Node*>();

	for (auto node : nodes)
	{
		if (node->is_leaf())
		{
			leaves.push_back(node);
		}
	}

	return leaves;
}


void Tree::dump_nodes(string file_name, vector<Node*> nodes)
{
	fstream file;
	file.open(file_name, std::fstream::out | std::fstream::trunc);
	//cout << "\n\n open file \n\n";
	for (auto node : nodes)
	{

		fstream last_node;
		last_node.open("last node", std::fstream::out | std::fstream::trunc);
		//cout << "\n\n getting nodes \n\n";
		vector<Segment_2> segs = get_bounding_box(node).cuts_to_segments();
		//cout << "got boundingbox";
		for (auto s : segs)
		{
			file << s << " " << node->priority() << "\n";
			last_node << s << "\n";
		}
		last_node.close();
		file << "\n";
	//	file << get_node_depth(node, 0) << "\n";

	}
	//cout << "\n\n wrote nodes \n\n";
	file.close();

}

void Tree::naive_insert(Segment* segment) 
{
	//std::cout << "hello from naive insert\n";
	vector<Node*> leaves = search(segment);


	vector<Node*> map = get_leaves();
	//dump_nodes("tree" + std::to_string(map.size()) + ".dat", map);
	//dump_nodes("leaves" + std::to_string(map.size()) + ".dat", leaves);
	//cout << "search complete\n";
	//cout << "found " << leaves.size() << " leaves\n";
	vector<Node*> nodes_to_be_partitioned = vector<Node*>();


	while (!leaves.empty())
	{
		//cout << "hello from insert\n";
		//cout << leaves.size() << "\n";
		//cout << segment->seg;
		Node* leaf = leaves[leaves.size() - 1];
		leaves.pop_back();

		CutType type = determine_vertical_cut_type(leaf, segment);

		if (type == VERTICAL_SOURCE)
		{
			//cout << "VERTICAL SOURCE\n";
			leaf->cut = new Cut();
			leaf->cut->type = VERTICAL_SOURCE;
			leaf->cut->segment = segment;
		}

		if (type == VERTICAL_TARGET)
		{
			//cout << "VERTICAL TARGET\n";
			leaf->cut = new Cut();
			leaf->cut->type = VERTICAL_TARGET;
			leaf->cut->segment = segment;
		}

		if (type == SEGMENT_INTERSECTION)
		{
			//cout << Tree::get_bounding_box(leaf);
			BoundingBox bounding_box = get_bounding_box(leaf);
			vector<Segment_2> bottom_top = bounding_box.get_top_and_bottom_segments();

			leaf->cut = new Cut();
			leaf->cut->type = SEGMENT_INTERSECTION;
			leaf->cut->segment = segment;
			Line_2 line = Line_2(leaf->cut->segment->seg);
			if (slope_of_line(line) > 0) 
			{
				if (check_for_proper_intersection(bottom_top[0], segment->seg))
				{
					//cout << "intersection with bottom\n";
					leaf->cut->intersecting_segment = bounding_box.bottom->segment;
				}
				else
				{
					//cout << "intersection with top\n";
					leaf->cut->intersecting_segment = bounding_box.top->segment;
				}
			}
			else 
			{
				if (check_for_proper_intersection(bottom_top[1], segment->seg))
				{
					leaf->cut->intersecting_segment = bounding_box.top->segment;
				}
				else
				{
					leaf->cut->intersecting_segment = bounding_box.bottom->segment;
				}
			}

		}

		if (type == NONE)
		{
			//cout << "NONE \n";
			//cout << get_bounding_box(leaf);
			if (segment_intersects_region(leaf, segment))
			{
				//cout << "intersects region\n";
				nodes_to_be_partitioned.push_back(leaf);
			}
			else
			{
				//cout << "does not intersect leaf\n";

				vector<Segment_2> segs = get_bounding_box(leaf).cuts_to_segments();

				for (auto s : segs)
				{
					//cout << s << "\n";
				}

			}
		}
		else
		{
			v_partition_naive(leaf, leaf->cut);
			leaves.push_back(leaf->positive_child);
			leaves.push_back(leaf->negative_child);
			nodes.push_back(leaf->negative_child);
			nodes.push_back(leaf->positive_child);
		}
	}

	for (int i = 0; i < nodes_to_be_partitioned.size(); ++i)
	{

		partition_naive(nodes_to_be_partitioned[i], segment);
		//cout << "partition " << i << "\n";
		nodes.push_back(nodes_to_be_partitioned[i]->negative_child);

		//cout << get_bounding_box(nodes_to_be_partitioned[i]->negative_child);
		vector<Segment_2> neg_segs = get_bounding_box(nodes_to_be_partitioned[i]->negative_child).cuts_to_segments();

		for (auto s : neg_segs)
		{
			//cout << s << "\n";
		}
		//cout << get_bounding_box(nodes_to_be_partitioned[i]->positive_child);

		vector<Segment_2> pos_segs = get_bounding_box(nodes_to_be_partitioned[i]->positive_child).cuts_to_segments();

		for (auto s : pos_segs)
		{
			//cout << s << "\n";
		}

		nodes.push_back(nodes_to_be_partitioned[i]->positive_child);
	}
}

void Tree::insert(Segment* segment)
{

	clock_t start;
	clock_t end;

	if (EXPERIMENTS == 1) 
	{
		start = clock();
		number_of_segments++;
	}

	seg_count++;

	vector<Node*> leaves = search_with_priority(segment);
	int number_of_leaves = leaves.size();

	vector<Node*> map = get_leaves();
	vector<Node*> nodes_to_be_partitioned = vector<Node*>();


	while (!leaves.empty())
	{
		Node* leaf = leaves[leaves.size() - 1];
		leaves.pop_back();

		CutType type = determine_vertical_cut_type(leaf, segment);

		if (type == VERTICAL_SOURCE)
		{
			leaf->cut = new Cut();
			leaf->cut->type = VERTICAL_SOURCE;
			leaf->cut->segment = segment;
		}

		if (type == VERTICAL_TARGET)
		{
			leaf->cut = new Cut();
			leaf->cut->type = VERTICAL_TARGET;
			leaf->cut->segment = segment;
		}

		if (type == SEGMENT_INTERSECTION)
		{
			BoundingBox bounding_box = get_bounding_box(leaf);
			vector<Segment_2> bottom_top = bounding_box.get_top_and_bottom_segments();

			leaf->cut = new Cut();
			leaf->cut->type = SEGMENT_INTERSECTION;
			leaf->cut->segment = segment;
			Line_2 line = Line_2(leaf->cut->segment->seg);
			
			if (check_for_proper_intersection(bottom_top[0], segment->seg)) 
			{
				if (check_for_proper_intersection(bottom_top[1], segment->seg)) 
				{
					Point_2 bottom_intersection = intersect_segments(bottom_top[0], segment->seg);
					Point_2 top_intersection = intersect_segments(bottom_top[1], segment->seg);

					if (bottom_intersection.x() < top_intersection.x()) 
					{
						leaf->cut->intersecting_segment = bounding_box.bottom->segment;
					}
					else 
					{
						leaf->cut->intersecting_segment = bounding_box.top->segment;
					}
				}
				else 
				{
					leaf->cut->intersecting_segment = bounding_box.bottom->segment;
				}
			}
			else if (check_for_proper_intersection(bottom_top[1], segment->seg))
			{
				leaf->cut->intersecting_segment = bounding_box.top->segment;
			}
			else 
			{
				std::cout << "\n expected segment intersection, none found \n";
				exit(0);
			}

			
			/*
			if (slope_of_line(line) > 0)
			{
				if (check_for_proper_intersection(bottom_top[0], segment->seg))
				{
					//cout << "intersection with bottom\n";
					leaf->cut->intersecting_segment = bounding_box.bottom->segment;
				}
				else
				{
					//cout << "intersection with top\n";
					leaf->cut->intersecting_segment = bounding_box.top->segment;
				}
			}
			else
			{
				if (check_for_proper_intersection(bottom_top[1], segment->seg))
				{
					leaf->cut->intersecting_segment = bounding_box.top->segment;
				}
				else
				{
					leaf->cut->intersecting_segment = bounding_box.bottom->segment;
				}
			}*/
		}

		if (type == NONE)
		{
			//cout << "NONE \n";
			//cout << get_bounding_box(leaf);
			if (segment_intersects_region(leaf, segment))
			{
				//cout << "intersects region\n";
				//cout << segment->seg << "\n";
				//cout << get_bounding_box(leaf) << "\n";
				nodes_to_be_partitioned.push_back(leaf);
			}
			else
			{
				//cout << "does not intersect leaf\n";
				vector<Segment_2> segs = get_bounding_box(leaf).cuts_to_segments();
			}
		}
		else
		{
			v_partition_naive(leaf, leaf->cut);
			leaves.push_back(leaf->positive_child);
			leaves.push_back(leaf->negative_child);
			nodes.push_back(leaf->negative_child);
			nodes.push_back(leaf->positive_child);
		}

		/*
		if (leaves.size() > 0) 
		{
			dump_nodes("leaves.dat", leaves);
		}

		if (nodes_to_be_partitioned.size() == 2) 
		{
			
		}

		if (nodes_to_be_partitioned.size() == 1) 
		{
			dump_nodes("test.dat", nodes_to_be_partitioned);
		}*/

		//cout << "nodes to be partitioned: " << nodes_to_be_partitioned.size() << "\n";
	}


	//cout << "nodes to be partitioned: " << nodes_to_be_partitioned.size();
	//dump_nodes("nodes_to_be_partitioned.dat", nodes_to_be_partitioned);

	for (int i = 0; i < nodes_to_be_partitioned.size(); ++i)
	{

		partition_naive(nodes_to_be_partitioned[i], segment);
		//cout << "partition " << i << "\n";
		nodes.push_back(nodes_to_be_partitioned[i]->negative_child);

		//cout << get_bounding_box(nodes_to_be_partitioned[i]->negative_child);
		vector<Segment_2> neg_segs = get_bounding_box(nodes_to_be_partitioned[i]->negative_child).cuts_to_segments();

		for (auto s : neg_segs)
		{
			//cout << s << "\n";
		}
		//cout << get_bounding_box(nodes_to_be_partitioned[i]->positive_child);

		vector<Segment_2> pos_segs = get_bounding_box(nodes_to_be_partitioned[i]->positive_child).cuts_to_segments();

		for (auto s : pos_segs)
		{
			//cout << s << "\n";
		}

		nodes.push_back(nodes_to_be_partitioned[i]->positive_child);
	}
	
	if (EXPERIMENTS == 1 && number_of_segments % step_size == 0) 
	{
		end = clock();
		dynamic_insert_stats << number_of_segments << "\t";
		dynamic_insert_stats << segment->priority << "\t";
		dynamic_insert_stats << number_of_leaves << "\t";
		dynamic_insert_stats << size(root) << "\t";
		dynamic_insert_stats << depth(root) << "\t";
		dynamic_insert_stats << nodes_visited_in_search << "\t";
		dynamic_insert_stats << v_partition_calls << "\t";
		dynamic_insert_stats << partition_calls << "\t";
		dynamic_insert_stats << v_merge_calls << "\t";
		dynamic_insert_stats << "0\t";
		dynamic_insert_stats << "0\t";
		dynamic_insert_stats << (double)(end - start) / CLOCKS_PER_SEC << "\n";
		//dynamic_insert_stats.close();

		nodes_visited_in_search = 0;
		v_partition_calls = 0;
		partition_calls = 0;
		v_merge_calls = 0;
	}

	if (CHECKING >= 100)
		if (!is_valid(root))
		{
			std::cout << "\n naive tree is not valid \n";
			exit(0);
		}
}

bool Tree::is_equal(Tree* other_tree)
{
	bool result = root->compare_nodes(other_tree->root);
	return result;
}

int Tree::count_intersections()
{
	int count = 0;
	for (int i = 0; i < nodes.size(); ++i)
	{
		if (nodes[i]->cut->type == SEGMENT_INTERSECTION)
		{
			count++;
		}
	}

	return count;
}

vector<Node*> Tree::naive_search_with_priority(Segment* segment)
{
	vector<Node*> output = vector<Node*>();

	for (int i = 0; i < nodes.size(); ++i)
	{
		if (segment_intersects_region(nodes[i], segment))
		{
			if (nodes[i]->priority() > segment->priority && nodes[i]->parent->priority() < segment->priority)
			{
				output.push_back(nodes[i]);
			}

			if (nodes[i]->parent == nullptr && nodes[i]->priority() > segment->priority)
			{
				output.push_back(nodes[i]);
			}
		}
	}

	return output;
}


// Returns true if the sum areas of the regions of the node's
// children is the same as the area of the node's region or
// if the node is a leaf.
bool is_node_valid(Node* node)
{
	return false;
}

bool Tree::is_valid(Node* node)
{
	if (node->is_leaf()) 
	{
		return true;
	}

	if (node->positive_child->parent != node) 
	{
		std::cout << "\n positive child does not point to parent \n";
		exit(0);
	}

	if (node->negative_child->parent != node) 
	{
		std::cout << "\n negative child does not point to parent \n";
		exit(0);
	}

	if (node->cut->type == SEGMENT) 
	{
		if (node->positive_child->priority() <= node->priority() || node->negative_child->priority() <= node->priority())
		{
			std::cout << "\n a node has lower priority than its parent destroyed by an edge cut\n";
			return false;
		}

		return is_valid(node->positive_child) && is_valid(node->negative_child);
	}
	else 
	{
		if (node->positive_child->priority() > node->priority() && node->negative_child->priority() > node->priority()) 
		{
			std::cout << "\n both children of a vertical cut node have strictly higher priority than their parent \n";
			return false;
		}

		return is_valid(node->positive_child) && is_valid(node->negative_child);
	}
	
}

Node* Tree::deepest_node_containing_segment(Node* node, Segment* segment) 
{
	if (EXPERIMENTS == 1)
		nodes_visited_in_search++;

	if (node->is_leaf() || node->priority() > segment->priority) 
	{
		return node;
	}

	if (segment_intersects_region(node->negative_child, segment) && segment_intersects_region(node->positive_child, segment)) 
	{
		return node;
	}

	if (segment_intersects_region(node->negative_child, segment)) 
	{
		return deepest_node_containing_segment(node->negative_child, segment);
	}

	return deepest_node_containing_segment(node->positive_child, segment);

}

bool node_requires_spitting(Node* node, Segment* segment) 
{

	if (node->is_leaf()) 
	{
		return false;
	}

	return node->priority() < segment->priority;
}

vector<Node*> Tree::search_with_priority(Segment* segment) 
{
	vector<Node*> output = vector<Node*>();
	Node* deepest_node = deepest_node_containing_segment(root, segment);

	std::stack<Node*> stack = std::stack<Node*>();
	stack.push(deepest_node);

	//int count = 0;

	while (!stack.empty()) 
	{
		if (EXPERIMENTS == 1)
			nodes_visited_in_search++;
		//count++;
	
		Node* top = stack.top();
		stack.pop();

		if (segment_intersects_region(top, segment)) 
		{
			if (node_requires_spitting(top, segment))
			{
				if (top->cut->type != SEGMENT)
				{
					stack.push(top->positive_child);
					stack.push(top->negative_child);
				}
				else
				{
					Line_2 defining_line = top->cut->get_defining_line();
					Line_2 segment_line = Line_2(segment->seg);

					if (slope_of_line(segment_line) > slope_of_line(defining_line))
					{
						stack.push(top->positive_child);
						stack.push(top->negative_child);
					}
					else
					{
						stack.push(top->negative_child);
						stack.push(top->positive_child);
					}
				}
			}
			else 
			{
				output.push_back(top);
			}
		}
	}
	//cout << seg_count << "\t" << nodes.size() << "\t" << count <<  "\t"  << (double)count / nodes.size() << "\n";

	return output;
}

Subtrees Tree::retrieve_subtrees(Node* node) 
{
	Subtrees subtrees = Subtrees();

	if (node->is_leaf()) 
	{
		subtrees.type = LEAF;
	}

	

	if (node->cut->type == VERTICAL_SOURCE) 
	{
		//std::cout << "\n VERTICAL SOURCE \n";
		//vector<Node*> leaves = vector<Node*>();

		//get_leaves_by_traversal(node, leaves);

		//dump_nodes("retrieve_subtrees_test.dat", leaves);
		
		if (node->positive_child->is_leaf()) 
		{

			//std::cout << "\n something is very wrong \n";
			std::cout << node->cut->segment->seg;
		}

		if (node->positive_child->cut->type == SEGMENT) 
		{
			//std::cout << "\n LAB \n";
			subtrees.type = LAB;
			subtrees.t_left = node->negative_child;
			subtrees.t_above = node->positive_child->positive_child;
			subtrees.t_below = node->positive_child->negative_child;
		}

		if (node->positive_child->cut->type == SEGMENT_INTERSECTION || node->positive_child->cut->type == VERTICAL_TARGET) 
		{
			//std::cout << "\n RLAB \n";
			subtrees.type = RLAB;
			subtrees.t_left = node->negative_child;
			subtrees.t_right = node->positive_child->positive_child;
			subtrees.t_above = node->positive_child->negative_child->positive_child;
			subtrees.t_below = node->positive_child->negative_child->negative_child;
		}
	}

	if (node->cut->type == VERTICAL_TARGET) 
	{
		//std::cout << "\n VERTICAL TARGET \n";
		subtrees.type = RAB;
		subtrees.t_right = node->positive_child;
		subtrees.t_above = node->negative_child->positive_child;
		subtrees.t_below = node->negative_child->negative_child;
	}


	if (node->cut->type == SEGMENT) 
	{
		//std::cout << "\n SEGMENT \n";
		subtrees.type = AB;
		subtrees.t_above = node->positive_child;
		subtrees.t_below = node->negative_child;
		//std::cout << "\n done \n";
	}

	if (node->cut->type == SEGMENT_INTERSECTION) 
	{

		if (node->negative_child->is_leaf() || node->negative_child->cut->segment != node->cut->segment) 
		{
			if (node->positive_child->cut->type == SEGMENT_INTERSECTION || node->positive_child->cut->type == VERTICAL_TARGET) 
			{
				subtrees.type = RLAB;
				subtrees.t_left = node->negative_child;
				subtrees.t_above = node->positive_child->negative_child->positive_child;
				subtrees.t_below = node->positive_child->negative_child->negative_child;
				subtrees.t_right = node->positive_child->positive_child;
			}
			else 
			{
				subtrees.type = LAB;
				subtrees.t_left = node->negative_child;
				subtrees.t_above = node->positive_child->positive_child;
				subtrees.t_below = node->positive_child->negative_child;
			}
		}
		else if (node->positive_child->is_leaf() || node->positive_child->cut->segment != node->cut->segment) 
		{
			subtrees.type = RAB;
			subtrees.t_right = node->positive_child;
			subtrees.t_above = node->negative_child->positive_child;
			subtrees.t_below = node->negative_child->negative_child;

			if (node->negative_child->cut->type != SEGMENT) 
			{
				std::cout << "\n\n\n SOMETHING VERY UNEXPECTED HAS HAPPENED!!! \n\n\n";
			}
		}
		else 
		{
			std::cout << "\n\n\n SOMETHING VERY UNEXPECTED HAS HAPPENED \n\n\n";
		}

		/*
		std::cout << "\n SEGMENT INTERSECTION \n";

		if (node->negative_child == nullptr) 
		{
			std::cout << "\n negative child is null \n";
		}


		if (node->negative_child->is_leaf() || node->negative_child->cut->segment != node->cut->segment) 
		{
			std::cout << "\n negative child is a leaf \n";
			if (node->positive_child->cut->type == VERTICAL_TARGET || node->positive_child->cut->type == SEGMENT_INTERSECTION) 
			{
				subtrees.type = RLAB;
				subtrees.t_left = node->negative_child;
				subtrees.t_above = node->positive_child->negative_child->positive_child;
				subtrees.t_below = node->positive_child->negative_child->negative_child;
				subtrees.t_right = node->positive_child->positive_child;
			}
			else 
			{
				subtrees.type = LAB;
				subtrees.t_left = node->negative_child;
				subtrees.t_above = node->positive_child->positive_child;
				subtrees.t_below = node->positive_child->negative_child;
			}
		}

		else if (node->positive_child->is_leaf() || node->positive_child->cut->segment != node->cut->segment) 
		{
			std::cout << "\n right tree is a leaf \n";
			subtrees.type = RAB;
			subtrees.t_right = node->positive_child;
			subtrees.t_above = node->negative_child->positive_child;
			subtrees.t_below = node->negative_child->negative_child;
			std::cout << "\n RAB set \n";
		}

		else if (node->positive_child->cut->type == SEGMENT_INTERSECTION || node->positive_child->cut->type == VERTICAL_TARGET) 
		{
			std::cout << "\n RLAB \n";
			subtrees.type = RLAB;
			subtrees.t_left = node->negative_child;
			subtrees.t_right = node->positive_child->positive_child;
			subtrees.t_above = node->positive_child->negative_child->positive_child;
			subtrees.t_below = node->positive_child->negative_child->negative_child;
		}

		else if (node->positive_child->cut->type == SEGMENT) 
		{
			std::cout << "\n LAB \n";
			subtrees.type = LAB;
			subtrees.t_left = node->negative_child;
			subtrees.t_above = node->positive_child->positive_child;
			subtrees.t_below = node->positive_child->negative_child;
		}


		//std::cout << *node->positive_child->cut;
		std::cout << "\n end of code block \n";
		*/
	}

	//std::cout << "\n about to return subtrees \n";
	return subtrees;
}

void Tree::increment_intersections() 
{
	if (EXPERIMENTS == 1) 
	{
		number_of_intersections++;
	}
}

vector<Cut*> Tree::get_cuts(Node* node, Segment* segment, Cut* carry_over, Cut* vertical_source, Cut* vertical_target, Cut* segment_cut) 
{
	//std::cout << "\n hello from get cuts \n";

	BoundingBox bounding_box = get_bounding_box(node);
	//cout << "\n" << segment->seg << "n";
	//cout << bounding_box;
	vector<Segment_2> bottom_top = bounding_box.get_top_and_bottom_segments();
	vector<Cut*> cuts = vector<Cut*>();
	
	Cut* left;
	Cut* right;
	Cut* seg_cut = segment_cut;

	if (check_for_proper_intersection(bottom_top[0], segment->seg) && check_for_proper_intersection(bottom_top[1], segment->seg)) 
	{
		Line_2 line = Line_2(segment->seg);
		
		if (carry_over != nullptr) 
		{
			if (carry_over->intersecting_segment == bounding_box.top->segment) 
			{
				Point_2 intersection_point = intersect_segments(segment->seg, bottom_top[0]);

				Line_2 carry_over_line = carry_over->get_defining_line();

				if (carry_over_line.has_on_positive_side(intersection_point)) 
				{
					left = carry_over;
					right = new Cut();
					right->type = SEGMENT_INTERSECTION;
					right->segment = segment;
					right->intersecting_segment = bounding_box.bottom->segment;
					increment_intersections();
				}
				else 
				{
					std::cout << "\n bug 1 \n";
					exit(0);
				}
			}
			else if (carry_over->intersecting_segment == bounding_box.bottom->segment) 
			{
				Point_2 intersection_point = intersect_segments(segment->seg, bottom_top[1]);

				Line_2 carry_over_line = carry_over->get_defining_line();

				if (carry_over_line.has_on_positive_side(intersection_point))
				{
					left = carry_over;
					right = new Cut();
					right->type = SEGMENT_INTERSECTION;
					right->segment = segment;
					right->intersecting_segment = bounding_box.top->segment;
					increment_intersections();
				}
				else
				{
					std::cout << "\n bug 2 \n";
					exit(0);
				}
			}
			else 
			{
				std::cout << "\n carry over error \n";
				std::cout << segment->seg << "\n";
				std::cout << carry_over->intersecting_segment->seg;
				exit(0);
			}
		}
		else 
		{
			std::cout << "\n cannot currently support endpoints in segments \n";
			std::cout << segment->seg;
			exit(0);
		}

		cuts.push_back(left);
		cuts.push_back(right);
		cuts.push_back(seg_cut);
		return cuts;
		/*
		Line_2 line = Line_2(segment->seg);

		if (slope_of_line(line) > 0) 
		{
			if (carry_over != nullptr) 
			{
				if (
					(carry_over->segment != segment) ||
					(carry_over->intersecting_segment != bounding_box.bottom->segment) ||
					(carry_over->type != SEGMENT_INTERSECTION)
					) 
				{
					std::cout << "\n bad carry over \n";
					cout << *carry_over;
					cout << segment->seg << "\n";
					cout << bounding_box.bottom->segment->seg << "\n";
					cout << bounding_box.top->segment->seg << "\n";
					exit(0);
				}

				left = carry_over;
			}
			else 
			{
				left = new Cut();
				left->segment = segment;
				left->type = SEGMENT_INTERSECTION;
				left->intersecting_segment = bounding_box.bottom->segment;
				//std::cout << "\n ITNERSECTION!! !! !! \n";
			}

			right = new Cut();
			right->segment = segment;
			right->type = SEGMENT_INTERSECTION;
			right->intersecting_segment = bounding_box.top->segment;
			//std::cout << "\n ITNERSECTION!! !! !! \n";

			cuts.push_back(left);
			cuts.push_back(right);
			cuts.push_back(seg_cut);
		}
		else 
		{

			if (carry_over != nullptr) 
			{
				left = carry_over;
			}
			else 
			{
				left = new Cut();
				left->segment = segment;
				left->type = SEGMENT_INTERSECTION;
				left->intersecting_segment = bounding_box.top->segment;
			}


			right = new Cut();
			right->segment = segment;
			right->type = SEGMENT_INTERSECTION;
			right->intersecting_segment = bounding_box.bottom->segment;
			//std::cout << "\n ITNERSECTION!! !! !! \n";
			cuts.push_back(left);
			cuts.push_back(right);
			cuts.push_back(seg_cut);
		}

		return cuts;*/
	}

	if (bounding_box.contains_point(segment->seg.source()) && bounding_box.contains_point(segment->seg.target())) 
	{

		left = vertical_source;
		right = vertical_target;

		cuts.push_back(left);
		cuts.push_back(right);
		cuts.push_back(seg_cut);

		return cuts;
	}

	if (bounding_box.contains_point(segment->seg.source()) && ((check_for_proper_intersection(bottom_top[1], segment->seg) || check_for_proper_intersection(bottom_top[0], segment->seg))))
	{
		left = vertical_source;
		right = new Cut();
		right->type = SEGMENT_INTERSECTION;
		right->segment = segment;
		increment_intersections();
		if (check_for_proper_intersection(bottom_top[0], segment->seg)) 
		{
			right->intersecting_segment = bounding_box.bottom->segment;
		}
		else 
		{
			right->intersecting_segment = bounding_box.top->segment;
		}

		cuts.push_back(left);
		cuts.push_back(right);
		cuts.push_back(seg_cut);

		return cuts;
	}

	if ((check_for_proper_intersection(bottom_top[0], segment->seg) || check_for_proper_intersection(bottom_top[1], segment->seg)) && bounding_box.contains_point_strict(segment->seg.target())) 
	{
		if (carry_over != nullptr) 
		{
			left = carry_over;
		}
		else 
		{
			left = new Cut();
			left->type = SEGMENT_INTERSECTION;
			left->segment = segment;
			increment_intersections();
			if (check_for_proper_intersection(bottom_top[0], segment->seg)) 
			{
				left->intersecting_segment = bounding_box.bottom->segment;
				
			}
			else 
			{
				left->intersecting_segment = bounding_box.top->segment;
			}
		}

		right = vertical_target;

		cuts.push_back(left);
		cuts.push_back(right);
		cuts.push_back(seg_cut);

		return cuts;
	}

	if ((check_for_proper_intersection(bottom_top[0], segment->seg) || check_for_proper_intersection(bottom_top[1], segment->seg)) && (!bounding_box.contains_point_strict(segment->seg.source()) && !bounding_box.contains_point_strict(segment->seg.target())))
	{
		if (check_for_proper_intersection(bottom_top[0], segment->seg)) 
		{
			if (carry_over != nullptr) 
			{
				cuts.push_back(carry_over);
			}
			else 
			{
				left = new Cut();
				left->type = SEGMENT_INTERSECTION;
				left->segment = segment;
				left->intersecting_segment = bounding_box.bottom->segment;
				increment_intersections();
				cuts.push_back(left);
			}

			cuts.push_back(segment_cut);
			return cuts;
		}
		else 
		{
			if (carry_over != nullptr)
			{
				cuts.push_back(carry_over);
			}
			else 
			{
				left = new Cut();
				left->type = SEGMENT_INTERSECTION;
				left->segment = segment;
				left->intersecting_segment = bounding_box.top->segment;
				increment_intersections();
				cuts.push_back(left);
			}
			cuts.push_back(segment_cut);
			return cuts;
		}
	}


	if (segment_intersects_region(node, segment)) 
	{
		if (region_contains_point(node, segment->seg.source())) 
		{
			cuts.push_back(vertical_source);
			cuts.push_back(segment_cut);
			return cuts;
		}

		if (region_contains_point(node, segment->seg.target())) 
		{
			cuts.push_back(vertical_target);
			cuts.push_back(segment_cut);
			return cuts;
		}

		cuts.push_back(segment_cut);
		return cuts;
	}
	
	std::cout << "\n this should never happen \n";
	exit(0);
	return cuts;
}

void Tree::v_partition_priority(Node* node, Cut* cut) 
{
	if (EXPERIMENTS == 1)
		v_partition_calls++;

	//std::cout << "\n vpart cut: \n" << *cut << "\n";
	if (cut->type == SEGMENT) 
	{
		cout << "\n can't v_partition with a segment cut \n";
		exit(0);
	}
	//std::cout << "hello from v-partition\n";

	if (node == nullptr) 
	{
		//std::cout << "\n v_partition called on null pointer \n";
	}

	if (node->is_leaf()) 
	{
		//std::cout << "\nsplitting a leaf\n";
		node->negative_child = new Node();
		node->negative_child->parent = node;
		node->cut = cut;

		node->positive_child = new Node();
		node->positive_child->parent = node;

		nodes.push_back(node->negative_child);
		nodes.push_back(node->positive_child);

		//std::cout << "\nfinished splitting a leaf\n";
		return;
	}

	//std::cout << "\n about to get subtrees \n";
	Subtrees subtrees = retrieve_subtrees(node);
	//std::cout << "\n got subtrees \n";
	//std::cout << subtrees.type;

	if (subtrees.type == AB) 
	{

		//std::cout << "\n AB case \n";

		Node* t_negative = new Node();
		Node* t_positive = new Node();

		t_negative->cut = node->cut;
		t_positive->cut = node->cut;

		t_negative->parent = node;
		t_positive->parent = node;

		v_partition_priority(subtrees.t_above, cut);
		v_partition_priority(subtrees.t_below, cut);

		t_negative->positive_child = subtrees.t_above->negative_child;
		t_negative->negative_child = subtrees.t_below->negative_child;

		subtrees.t_above->negative_child->parent = t_negative;
		subtrees.t_below->negative_child->parent = t_negative;

		t_positive->positive_child = subtrees.t_above->positive_child;
		t_positive->negative_child = subtrees.t_below->positive_child;

		subtrees.t_above->positive_child->parent = t_positive;
		subtrees.t_below->positive_child->parent = t_positive;

		node->negative_child = t_negative;
		node->positive_child = t_positive;
		node->cut = cut;

		// 
		//	remember to free memory for subtrees.t_above, subtrees.t_below
		//
	}

	if (subtrees.type == LAB) 
	{
		BoundingBox left_bounding_box = get_bounding_box(subtrees.t_left);

		if (left_bounding_box.vertical_line_cuts_region(cut->get_defining_line())) 
		{

			//std::cout << "\n LAB left case \n";

			Node* t_negative;
			Node* t_positive;
			v_partition_priority(subtrees.t_left, cut);

			t_negative = subtrees.t_left->negative_child;
			subtrees.t_left->negative_child->parent = t_negative;
			t_negative->parent = node;

			t_positive = new Node();
			t_positive->cut = node->cut;
			t_positive->negative_child = subtrees.t_left->positive_child;
			subtrees.t_left->positive_child->parent = t_positive;
			t_positive->positive_child = subtrees.t_above->parent;
			subtrees.t_above->parent->parent = t_positive;
			t_positive->parent = node;

			node->negative_child = t_negative;
			node->positive_child = t_positive;
			node->cut = cut;
		}
		else 
		{

		//	std::cout << "\n LAB not left case \n";

			v_partition_priority(subtrees.t_above, cut);
			v_partition_priority(subtrees.t_below, cut);

			Node* t_positive = new Node();
			Node* t_negative = new Node();
			Node* t_negative_positive = new Node();

			t_negative->cut = node->cut;
			t_negative->parent = node;
			t_negative->negative_child = subtrees.t_left;
			subtrees.t_left->parent = t_negative;
			t_negative->positive_child = t_negative_positive;
			t_negative_positive->parent = t_negative;
			t_negative_positive->cut = subtrees.t_above->parent->cut;

			t_negative_positive->negative_child = subtrees.t_below->negative_child;
			subtrees.t_below->negative_child->parent = t_negative_positive;
			t_negative_positive->positive_child = subtrees.t_above->negative_child;
			subtrees.t_above->negative_child->parent = t_negative_positive;

			t_positive->cut = subtrees.t_above->parent->cut;
			t_positive->parent = node;
			t_positive->negative_child = subtrees.t_below->positive_child;
			subtrees.t_below->positive_child->parent = t_positive;
			t_positive->positive_child = subtrees.t_above->positive_child;
			subtrees.t_above->positive_child->parent = t_positive;

			node->negative_child = t_negative;
			node->positive_child = t_positive;
			node->cut = cut;
		}
	}

	if (subtrees.type == RAB) 
	{
		//std::cout << "\n about to get bounding box \n";


		BoundingBox right_bounding_box = get_bounding_box(subtrees.t_right);


		if (right_bounding_box.vertical_line_cuts_region(cut->get_defining_line())) 
		{
		//	std::cout << "\n RAB right case \n";

			Node* t_positive;
			Node* t_negative;

			v_partition_priority(subtrees.t_right, cut);
			
			t_positive = subtrees.t_right->positive_child;
			t_positive->parent = node;

			t_negative = new Node();
			t_negative->cut = node->cut;
			t_negative->parent = node;

			t_negative->positive_child = subtrees.t_right->negative_child;
			subtrees.t_right->negative_child->parent = t_negative;
			t_negative->negative_child = subtrees.t_above->parent;
			subtrees.t_above->parent->parent = t_negative;

			node->negative_child = t_negative;
			node->positive_child = t_positive;
			node->cut = cut;

		}
		else 
		{

		//	std::cout << "\n RAB not right case \n";

			Node* t_negative;
			Node* t_positive;
			Node* t_positive_negative;

			v_partition_priority(subtrees.t_below, cut);
			v_partition_priority(subtrees.t_above, cut);

			t_negative = new Node();
			t_negative->parent = node;
			t_negative->cut = subtrees.t_above->parent->cut;
			t_negative->negative_child = subtrees.t_below->negative_child;
			subtrees.t_below->negative_child->parent = t_negative;
			t_negative->positive_child = subtrees.t_above->negative_child;
			subtrees.t_above->negative_child->parent = t_negative;


			t_positive_negative = new Node();
			t_positive_negative->cut = subtrees.t_above->parent->cut;
			t_positive_negative->negative_child = subtrees.t_below->positive_child;
			subtrees.t_below->positive_child->parent = t_positive_negative;
			t_positive_negative->positive_child = subtrees.t_above->positive_child;
			subtrees.t_above->positive_child->parent = t_positive_negative;

			t_positive = new Node();
			t_positive->cut = node->cut;
			t_positive->parent = node;
			t_positive->positive_child = subtrees.t_right;
			subtrees.t_right->parent = t_positive;
			t_positive->negative_child = t_positive_negative;
			t_positive_negative->parent = t_positive;


			node->negative_child = t_negative;
			node->positive_child = t_positive;
			node->cut = cut;
			
		}
	}

	if (subtrees.type == RLAB) 
	{
		BoundingBox above_box = get_bounding_box(subtrees.t_above);

		if (above_box.vertical_line_cuts_region(cut->get_defining_line())) 
		{

			//std::cout << "\n RLAB above case \n";

			Node* t_negative = new Node();
			Node* t_positive = new Node();
			Node* t_negative_positive = new Node();
			Node* t_positive_negative = new Node();


			v_partition_priority(subtrees.t_above, cut);
			v_partition_priority(subtrees.t_below, cut);

			t_negative->cut = node->cut;
			t_negative->parent = node;
			t_negative->negative_child = subtrees.t_left;
			subtrees.t_left->parent = t_negative;
			t_negative->positive_child = t_negative_positive;
			t_negative_positive->parent = t_negative;
			t_negative_positive->cut = subtrees.t_above->parent->cut;
			t_negative_positive->negative_child = subtrees.t_below->negative_child;
			subtrees.t_below->negative_child->parent = t_negative_positive;
			t_negative_positive->positive_child = subtrees.t_above->negative_child;
			subtrees.t_above->negative_child->parent = t_negative_positive;

			t_positive->cut = subtrees.t_right->parent->cut;
			t_positive->parent = node;
			t_positive->positive_child = subtrees.t_right;
			subtrees.t_right->parent = t_positive;
			t_positive->negative_child = t_positive_negative;
			t_positive_negative->parent = t_positive;
			t_positive_negative->cut = subtrees.t_above->parent->cut;
			t_positive_negative->negative_child = subtrees.t_below->positive_child;
			subtrees.t_below->positive_child->parent = t_positive_negative;
			t_positive_negative->positive_child = subtrees.t_above->positive_child;
			subtrees.t_above->positive_child->parent = t_positive_negative;

			node->positive_child = t_positive;
			node->negative_child = t_negative;
			node->cut = cut;
		}
		else 
		{
			BoundingBox left_box = get_bounding_box(subtrees.t_left);

			if (left_box.vertical_line_cuts_region(cut->get_defining_line())) 
			{

				//std::cout << "\n RLAB leftl case \n";

				Node* t_negative;
				Node* t_positive = new Node();

				v_partition_priority(subtrees.t_left, cut);

				t_negative = subtrees.t_left->negative_child;
				t_negative->parent = node;
				
				t_positive->cut = node->cut;
				t_positive->parent = node;
				t_positive->negative_child = subtrees.t_left->positive_child;
				subtrees.t_left->positive_child->parent = t_positive;

				t_positive->positive_child = node->positive_child;
				node->positive_child->parent = t_positive;

				node->positive_child = t_positive;
				node->negative_child = t_negative;
				node->cut = cut;
			}
			else 
			{

				//std::cout << "\n RLAB right case \n";

				Node* t_positive;
				Node* t_negative = new Node();
				Node* t_negative_positive = new Node();

				v_partition_priority(subtrees.t_right, cut);

				t_positive = subtrees.t_right->positive_child;
				t_positive->parent = node;
				subtrees.t_right->parent = t_positive;

				//std::cout << "\n positive set \n";

				t_negative->parent = node;
				t_negative->cut = node->cut;
				t_negative->negative_child = subtrees.t_left;
				subtrees.t_left->parent = t_negative;

				//std::cout << "\n negative set \n";

				t_negative->positive_child = t_negative_positive;
				t_negative_positive->parent = t_negative;
				t_negative_positive->cut = subtrees.t_above->parent->parent->cut;
				t_negative_positive->positive_child = subtrees.t_right->negative_child;
				subtrees.t_right->negative_child->parent = t_negative_positive;
				t_negative_positive->negative_child = subtrees.t_above->parent;
				subtrees.t_above->parent->parent = t_negative_positive;

				node->negative_child = t_negative;
				node->positive_child = t_positive;
				node->cut = cut;
			}
		}
	}
}

void Tree::partition_priority(Node* node, Cut* cut) 
{
	if (EXPERIMENTS == 1)
		partition_calls++;

	if (cut->type != SEGMENT) 
	{
		std::cout << "\n can't partition with a vertical cut...\n";
		exit(0);
	}
	if (node->is_leaf()) 
	{
		//std::cout << "\n\n PARTITION LEAF CASE \n\n";

		Node* t_positive = new Node();
		Node* t_negative = new Node();

		t_negative->parent = node;
		t_positive->parent = node;

		node->negative_child = t_negative;
		node->positive_child = t_positive;
		node->cut = cut;

		nodes.push_back(t_positive);
		nodes.push_back(t_negative);
		return;
	}

	Subtrees subtrees = retrieve_subtrees(node);

	if (segment_intersects_region(subtrees.t_above, cut->segment) && segment_intersects_region(subtrees.t_below, cut->segment)) 
	{

		if (EXPERIMENTS == 1) 
		{
			number_of_intersections++;
		}

		Cut* intersection_cut = new Cut();
		intersection_cut->type = SEGMENT_INTERSECTION;
		intersection_cut->segment = subtrees.t_above->parent->cut->segment;
		intersection_cut->intersecting_segment = cut->segment;
		
		Line_2 cut_line = cut->get_defining_line();
		Line_2 intersecting_line = subtrees.t_above->parent->cut->get_defining_line();
		
		if (subtrees.type == RLAB) 
		{
			Node* t_positive = new Node();
			Node* t_negative = new Node();

			v_partition_priority(subtrees.t_above, intersection_cut);
			v_partition_priority(subtrees.t_below, intersection_cut);

			if (slope_of_line(cut_line) > slope_of_line(intersecting_line)) 
			{
				partition_priority(subtrees.t_left, cut);
				partition_priority(subtrees.t_right, cut);
				partition_priority(subtrees.t_above->positive_child, cut);
				partition_priority(subtrees.t_below->negative_child, cut);

				t_positive->parent = node;
				t_positive->cut = node->cut;
				t_positive->negative_child = subtrees.t_left->positive_child;
				t_positive->negative_child->parent = t_positive;
				Node* t_positive_positive = new Node();
				t_positive_positive->cut = intersection_cut;
				Node* t_positive_positive_negative = new Node();
				t_positive_positive_negative->cut = subtrees.t_above->parent->cut;
				t_positive_positive_negative->positive_child = subtrees.t_above->negative_child;
				t_positive_positive_negative->positive_child->parent = t_positive_positive_negative;
				t_positive_positive_negative->negative_child = subtrees.t_below->negative_child->positive_child;
				t_positive_positive_negative->negative_child->parent = t_positive_positive_negative;
				t_positive_positive->negative_child = t_positive_positive_negative;
				t_positive_positive_negative->parent = t_positive_positive;
				Node* merge_above_right = new Node();
				merge_above_right->cut = subtrees.t_right->parent->cut;
				merge_above_right->negative_child = subtrees.t_above->positive_child->positive_child;
				merge_above_right->negative_child->parent = merge_above_right;
				merge_above_right->positive_child = subtrees.t_right->positive_child;
				merge_above_right->positive_child->parent = merge_above_right;
				v_merge_priority(merge_above_right);
				t_positive_positive->positive_child = merge_above_right;
				merge_above_right->parent = t_positive_positive;
				t_positive_positive->parent = t_positive;
				t_positive->positive_child = t_positive_positive;

				t_negative->cut = intersection_cut;
				t_negative->parent = node;
				Node* t_negative_positive = new Node();
				t_negative_positive->cut = subtrees.t_right->parent->cut;
				t_negative_positive->parent = t_negative;
				t_negative->positive_child = t_negative_positive;
				t_negative_positive->positive_child = subtrees.t_right->negative_child;
				subtrees.t_right->negative_child->parent = t_negative_positive;
				Node* t_negative_positive_negative = new Node();
				t_negative_positive_negative->cut = subtrees.t_above->parent->cut;
				t_negative_positive_negative->positive_child = subtrees.t_above->positive_child->negative_child;
				t_negative_positive_negative->positive_child->parent = t_negative_positive_negative;
				t_negative_positive_negative->negative_child = subtrees.t_below->positive_child;
				t_negative_positive_negative->negative_child->parent = t_negative_positive_negative;
				
				t_negative_positive->negative_child = t_negative_positive_negative;
				t_negative_positive_negative->parent = t_negative_positive;

				Node* merge_below_left = new Node();
				merge_below_left->cut = node->cut;
				merge_below_left->negative_child = subtrees.t_left->negative_child;
				merge_below_left->negative_child->parent = merge_below_left;
				merge_below_left->positive_child = subtrees.t_below->negative_child->negative_child;
				merge_below_left->positive_child->parent = merge_below_left;
				v_merge_priority(merge_below_left);

				t_negative->negative_child = merge_below_left;
				merge_below_left->parent = t_negative;
			}
			else 
			{
				partition_priority(subtrees.t_left, cut);
				partition_priority(subtrees.t_right, cut);
				partition_priority(subtrees.t_above->negative_child, cut);
				partition_priority(subtrees.t_below->positive_child, cut);

				t_positive->cut = intersection_cut;
				t_positive->parent = node;
				Node* t_positive_positive = new Node();
				t_positive_positive->cut = subtrees.t_right->parent->cut;
				t_positive->positive_child = t_positive_positive;
				t_positive_positive->parent = t_positive;
				t_positive_positive->positive_child = subtrees.t_right->positive_child;
				t_positive_positive->positive_child->parent = t_positive_positive;
				Node* t_positive_positive_negative = new Node();
				t_positive_positive_negative->cut = subtrees.t_above->parent->cut;
				t_positive_positive_negative->positive_child = subtrees.t_above->positive_child;
				t_positive_positive_negative->positive_child->parent = t_positive_positive_negative;
				t_positive_positive_negative->negative_child = subtrees.t_below->positive_child->positive_child;
				t_positive_positive_negative->negative_child->parent = t_positive_positive_negative;
				t_positive_positive->negative_child = t_positive_positive_negative;
				t_positive_positive_negative->parent = t_positive_positive;

				Node* merge_left_above = new Node();
				merge_left_above->cut = node->cut;
				merge_left_above->negative_child = subtrees.t_left->positive_child;
				merge_left_above->negative_child->parent = merge_left_above;
				merge_left_above->positive_child = subtrees.t_above->negative_child->positive_child;
				merge_left_above->positive_child->parent = merge_left_above;

				v_merge_priority(merge_left_above);

				t_positive->negative_child = merge_left_above;
				merge_left_above->parent = t_positive;

				t_negative->cut = node->cut;
				t_negative->parent = node;
				t_negative->negative_child = subtrees.t_left->negative_child;
				t_negative->negative_child->parent = t_negative;
				Node* t_negative_positive = new Node();
				t_negative_positive->cut = intersection_cut;
				t_negative->positive_child = t_negative_positive;
				t_negative_positive->parent = t_negative;
				Node* t_negative_positive_negative = new Node();
				t_negative_positive_negative->cut = subtrees.t_above->parent->cut;
				t_negative_positive_negative->positive_child = subtrees.t_above->negative_child->negative_child;
				t_negative_positive_negative->positive_child->parent = t_negative_positive_negative;
				t_negative_positive_negative->negative_child = subtrees.t_below->negative_child;
				t_negative_positive_negative->negative_child->parent = t_negative_positive_negative;

				t_negative_positive->negative_child = t_negative_positive_negative;
				t_negative_positive_negative->parent = t_negative_positive;

				Node* merge_below_right = new Node();
				merge_below_right->cut = subtrees.t_right->parent->cut;
				merge_below_right->negative_child = subtrees.t_below->positive_child->negative_child;
				merge_below_right->negative_child->parent = merge_below_right;
				merge_below_right->positive_child = subtrees.t_right->negative_child;
				merge_below_right->positive_child->parent = merge_below_right;

				v_merge_priority(merge_below_right);

				t_negative_positive->positive_child = merge_below_right;
				merge_below_right->parent = t_negative_positive;
			}

			node->positive_child = t_positive;
			node->negative_child = t_negative;
			node->cut = cut;
			return;
		}
		else if (subtrees.type == LAB) 
		{
			//cout << "\n\n IN THE EXPECTED CASE\n\n";
			Node* t_positive = new Node();
			Node* t_negative = new Node();

			v_partition_priority(subtrees.t_above, intersection_cut);
			v_partition_priority(subtrees.t_below, intersection_cut);

			if (slope_of_line(cut_line) > slope_of_line(intersecting_line)) 
			{
				partition_priority(subtrees.t_above->positive_child, cut);
				partition_priority(subtrees.t_below->negative_child, cut);
				partition_priority(subtrees.t_left, cut);

				t_positive->cut = node->cut;
				t_positive->parent = node;
				t_positive->negative_child = subtrees.t_left->positive_child;
				t_positive->negative_child->parent = t_positive;
				Node* t_positive_positive = new Node();
				t_positive_positive->cut = intersection_cut;
				t_positive->positive_child = t_positive_positive;
				t_positive_positive->parent = t_positive;
				t_positive_positive->positive_child = subtrees.t_above->positive_child->positive_child;
				t_positive_positive->positive_child->parent = t_positive_positive;

				Node* t_positive_positive_negative = new Node();
				t_positive_positive_negative->cut = subtrees.t_above->parent->cut;
				t_positive_positive_negative->positive_child = subtrees.t_above->negative_child;
				t_positive_positive_negative->positive_child->parent = t_positive_positive_negative;
				t_positive_positive_negative->negative_child = subtrees.t_below->negative_child->positive_child;
				t_positive_positive_negative->negative_child->parent = t_positive_positive_negative;
				t_positive_positive->negative_child = t_positive_positive_negative;
				t_positive_positive_negative->parent = t_positive_positive;
				
				t_negative->cut = intersection_cut;
				t_negative->parent = node;
				Node* t_negative_positive = new Node();
				t_negative_positive->cut = subtrees.t_above->parent->cut;
				t_negative_positive->positive_child = subtrees.t_above->positive_child->negative_child;
				t_negative_positive->positive_child->parent = t_negative_positive;
				t_negative_positive->negative_child = subtrees.t_below->positive_child;
				t_negative_positive->negative_child->parent = t_negative_positive;
				t_negative->positive_child = t_negative_positive;
				t_negative_positive->parent = t_negative;

				Node* merge_left_below = new Node();
				merge_left_below->cut = node->cut;
				merge_left_below->negative_child = subtrees.t_left->negative_child;
				merge_left_below->negative_child->parent = merge_left_below;
				merge_left_below->positive_child = subtrees.t_below->negative_child->negative_child;
				merge_left_below->positive_child->parent = merge_left_below;

				v_merge_priority(merge_left_below);
				
				merge_left_below->parent = t_negative;
				t_negative->negative_child = merge_left_below;

				node->positive_child = t_positive;
				node->negative_child = t_negative;
				node->cut = cut;
				return;
			}
			else 
			{
				partition_priority(subtrees.t_above->negative_child, cut);
				partition_priority(subtrees.t_below->positive_child, cut);
				partition_priority(subtrees.t_left, cut);


				t_negative->parent = node;
				t_negative->cut = node->cut;

				t_negative->negative_child = subtrees.t_left->negative_child;
				t_negative->negative_child->parent = t_negative;
				Node* t_negative_positive = new Node();
				t_negative_positive->cut = intersection_cut;
				t_negative_positive->parent = t_negative;
				t_negative->positive_child = t_negative_positive;
				t_negative_positive->positive_child = subtrees.t_below->positive_child->negative_child;

				t_negative_positive->positive_child->parent = t_negative_positive;
				Node* t_negative_positive_negative = new Node();
				t_negative_positive_negative->cut = subtrees.t_above->parent->cut;
				t_negative_positive_negative->parent = t_negative_positive;
				t_negative_positive->negative_child = t_negative_positive_negative;
				t_negative_positive_negative->positive_child = subtrees.t_above->negative_child->negative_child;
				t_negative_positive_negative->positive_child->parent = t_negative_positive_negative;
				t_negative_positive_negative->negative_child = subtrees.t_below->negative_child;
				t_negative_positive_negative->negative_child->parent = t_negative_positive_negative;

				t_positive->cut = intersection_cut;
				t_positive->parent = node;
				Node* t_positive_positive = new Node();
				t_positive_positive->cut = subtrees.t_above->parent->cut;
				t_positive_positive->parent = t_positive;
				t_positive->positive_child = t_positive_positive;
				t_positive_positive->positive_child = subtrees.t_above->positive_child;
				t_positive_positive->positive_child->parent = t_positive_positive;
				t_positive_positive->negative_child = subtrees.t_below->positive_child->positive_child;
				t_positive_positive->negative_child->parent = t_positive_positive;

				Node* merge_left_above = new Node();
				merge_left_above->cut = node->cut;
				merge_left_above->negative_child = subtrees.t_left->positive_child;
				merge_left_above->negative_child->parent = merge_left_above;
				merge_left_above->positive_child = subtrees.t_above->negative_child->positive_child;
				merge_left_above->positive_child->parent = merge_left_above;

				v_merge_priority(merge_left_above);

				merge_left_above->parent = t_positive;
				t_positive->negative_child = merge_left_above;

				node->negative_child = t_negative;
				node->positive_child = t_positive;
				node->cut = cut;
			}
		}
		else if (subtrees.type == RAB) 
		{
			Node* t_positive = new Node();
			Node* t_negative = new Node();

			Cut* intersection_cut = new Cut();
			intersection_cut->type = SEGMENT_INTERSECTION;
			intersection_cut->segment = subtrees.t_above->parent->cut->segment;
			intersection_cut->intersecting_segment = cut->segment;

			v_partition_priority(subtrees.t_above, intersection_cut);
			v_partition_priority(subtrees.t_below, intersection_cut);

			Line_2 cut_line = cut->get_defining_line();
			Line_2 intersecting_line = subtrees.t_above->parent->cut->get_defining_line();

			if (slope_of_line(cut_line) > slope_of_line(intersecting_line)) 
			{
				partition_priority(subtrees.t_above->positive_child, cut);
				partition_priority(subtrees.t_below->negative_child, cut);
				partition_priority(subtrees.t_right, cut);

				t_positive->cut = intersection_cut;
				t_positive->parent = node;

				Node* t_positive_negative = new Node();
				t_positive_negative->cut = subtrees.t_above->parent->cut;
				t_positive_negative->positive_child = subtrees.t_above->negative_child;
				t_positive_negative->positive_child->parent = t_positive_negative;
				t_positive_negative->negative_child = subtrees.t_below->negative_child->positive_child;
				t_positive_negative->negative_child->parent = t_positive_negative;

				t_positive->negative_child = t_positive_negative;
				t_positive->negative_child->parent = t_positive;
				
				Node* merge_above_right = new Node();
				merge_above_right->cut = node->cut;
				merge_above_right->negative_child = subtrees.t_above->positive_child->positive_child;
				merge_above_right->negative_child->parent = merge_above_right;
				merge_above_right->positive_child = subtrees.t_right->positive_child;
				merge_above_right->positive_child->parent = merge_above_right;

				v_merge_priority(merge_above_right);

				t_positive->positive_child = merge_above_right;
				t_positive->positive_child->parent = t_positive;

				
				t_negative->cut = intersection_cut;
				t_negative->parent = node;
				t_negative->negative_child = subtrees.t_below->negative_child->negative_child;
				t_negative->negative_child->parent = t_negative;

				Node* t_negative_positive = new Node();
				t_negative_positive->cut = node->cut;
				t_negative_positive->parent = t_negative;
				t_negative->positive_child = t_negative_positive;
				t_negative_positive->positive_child = subtrees.t_right->negative_child;
				t_negative_positive->positive_child->parent = t_negative_positive;

				Node* t_negative_positive_negative = new Node();
				t_negative_positive_negative->cut = subtrees.t_above->parent->cut;
				t_negative_positive_negative->positive_child = subtrees.t_above->positive_child->negative_child;
				t_negative_positive_negative->positive_child->parent = t_negative_positive_negative;
				t_negative_positive_negative->negative_child = subtrees.t_below->positive_child;
				t_negative_positive_negative->negative_child->parent = t_negative_positive_negative;

				t_negative_positive->negative_child = t_negative_positive_negative;
				t_negative_positive->negative_child->parent = t_negative_positive;

				node->negative_child = t_negative;
				node->positive_child = t_positive;
				node->cut = cut;
			}
			else 
			{
				partition_priority(subtrees.t_above->negative_child, cut);
				partition_priority(subtrees.t_below->positive_child, cut);
				partition_priority(subtrees.t_right, cut);
				//cout << "\n hello!!!! \n";
				t_positive->parent = node;
				t_positive->cut = intersection_cut;

				t_positive->negative_child = subtrees.t_above->negative_child->positive_child;
				t_positive->negative_child->parent = t_positive;

				Node* t_positive_positive = new Node();
				t_positive_positive->cut = node->cut;
				t_positive_positive->parent = t_positive;
				t_positive->positive_child = t_positive_positive;

				t_positive_positive->positive_child = subtrees.t_right->positive_child;
				t_positive_positive->positive_child->parent = t_positive_positive;

				Node* t_positive_positive_negative = new Node();
				t_positive_positive_negative->cut = subtrees.t_above->parent->cut;
				t_positive_positive_negative->parent = t_positive_positive;
				t_positive_positive->negative_child = t_positive_positive_negative;

				t_positive_positive_negative->positive_child = subtrees.t_above->positive_child;
				t_positive_positive_negative->positive_child->parent = t_positive_positive_negative;
				t_positive_positive_negative->negative_child = subtrees.t_below->positive_child->positive_child;
				t_positive_positive_negative->negative_child->parent = t_positive_positive_negative;

				//cout << "\n hello!111 \n";
				t_negative->cut = intersection_cut;
				t_negative->parent = node;

				Node* t_negative_negative = new Node();
				t_negative_negative->cut = subtrees.t_above->parent->cut;
				t_negative_negative->parent = t_negative;
				t_negative->negative_child = t_negative_negative;
				t_negative_negative->positive_child = subtrees.t_above->negative_child->negative_child;
				t_negative_negative->positive_child->parent = t_negative_negative;
				t_negative_negative->negative_child = subtrees.t_below->negative_child;
				t_negative_negative->negative_child->parent = t_negative_negative;
				//cout << "\n hello!! \n";
				Node* merge_below_right = new Node();
				merge_below_right->cut = node->cut;
				merge_below_right->positive_child = subtrees.t_right->negative_child;
				merge_below_right->positive_child->parent = merge_below_right;
				merge_below_right->negative_child = subtrees.t_below->positive_child->negative_child;
				merge_below_right->negative_child->parent = merge_below_right;

				v_merge_priority(merge_below_right);

				t_negative->positive_child = merge_below_right;
				merge_below_right->parent = t_negative;
				//cout << "\n hello! \n";
				node->negative_child = t_negative;
				node->positive_child = t_positive;
				node->cut = cut;
			}
		}
		else if (subtrees.type == AB) 
		{
			Node* t_positive = new Node();
			Node* t_negative = new Node();

			Cut* intersection_cut = new Cut();
			intersection_cut->type = SEGMENT_INTERSECTION;
			intersection_cut->segment = subtrees.t_above->parent->cut->segment;
			intersection_cut->intersecting_segment = cut->segment;

			v_partition_priority(subtrees.t_above, intersection_cut);
			v_partition_priority(subtrees.t_below, intersection_cut);

			Line_2 cut_line = cut->get_defining_line();
			Line_2 intersecting_line = subtrees.t_above->parent->cut->get_defining_line();

			t_positive->parent = node;
			t_negative->parent = node;

			t_positive->cut = intersection_cut;
			t_negative->cut = intersection_cut;

			if (slope_of_line(cut_line) > slope_of_line(intersecting_line)) 
			{
				partition_priority(subtrees.t_above->positive_child, cut);
				partition_priority(subtrees.t_below->negative_child, cut);

				t_positive->positive_child = subtrees.t_above->positive_child->positive_child;
				t_positive->positive_child->parent = t_positive;

				Node* t_positive_negative = new Node();
				t_positive_negative->cut = subtrees.t_above->parent->cut;
				t_positive->negative_child = t_positive_negative;
				t_positive_negative->parent = t_positive;

				t_positive_negative->positive_child = subtrees.t_above->negative_child;
				t_positive_negative->positive_child->parent = t_positive_negative;
				t_positive_negative->negative_child = subtrees.t_below->negative_child->positive_child;
				t_positive_negative->negative_child->parent = t_positive_negative;

				t_negative->negative_child = subtrees.t_below->negative_child->negative_child;
				t_negative->negative_child->parent = t_negative;
				Node* t_negative_positive = new Node();
				t_negative_positive->cut = subtrees.t_above->parent->cut;
				t_negative->positive_child = t_negative_positive;
				t_negative_positive->parent = t_negative;
				t_negative_positive->positive_child = subtrees.t_above->positive_child->negative_child;
				t_negative_positive->positive_child->parent = t_negative_positive;
				t_negative_positive->negative_child = subtrees.t_below->positive_child;
				t_negative_positive->negative_child->parent = t_negative_positive;

			}
			else 
			{
				partition_priority(subtrees.t_above->negative_child, cut);
				partition_priority(subtrees.t_below->positive_child, cut);

				t_positive->negative_child = subtrees.t_above->negative_child->positive_child;
				t_positive->negative_child->parent = t_positive;
				Node* t_positive_positive = new Node();
				t_positive_positive->cut = subtrees.t_above->parent->cut;
				t_positive->positive_child = t_positive_positive;
				t_positive_positive->parent = t_positive;
				t_positive_positive->positive_child = subtrees.t_above->positive_child;
				t_positive_positive->positive_child->parent = t_positive_positive;
				t_positive_positive->negative_child = subtrees.t_below->positive_child->positive_child;
				t_positive_positive->negative_child->parent = t_positive_positive;

				t_negative->positive_child = subtrees.t_below->positive_child->negative_child;
				t_negative->positive_child->parent = t_negative;
				
				Node* t_negative_negative = new Node();
				t_negative_negative->cut = subtrees.t_above->parent->cut;
				t_negative->negative_child = t_negative_negative;
				t_negative_negative->parent = t_negative;
				t_negative_negative->negative_child = subtrees.t_below->negative_child;
				t_negative_negative->negative_child->parent = t_negative_negative;
				t_negative_negative->positive_child = subtrees.t_above->negative_child->negative_child;
				t_negative_negative->positive_child->parent = t_negative_negative;
				
			}


			node->positive_child = t_positive;
			node->negative_child = t_negative;
			node->cut = cut;
		}
		return;
	}


	if (subtrees.type == AB) 
	{
		/*
			ADD SEEGMENT INTERSECTION CASE
		*/
		//std::cout << "\n\n PARTITION AB \n\n";
		if (segment_intersects_region(subtrees.t_above, cut->segment)) 
		{
			partition_priority(subtrees.t_above, cut);

			Node* t_negative = new Node();
			Node* t_positive;
			t_positive = subtrees.t_above->positive_child;
			t_positive->parent = node;
			t_negative->parent = node;
			t_negative->cut = node->cut;
			t_negative->positive_child = subtrees.t_above->negative_child;
			subtrees.t_above->negative_child->parent = t_negative;
			t_negative->negative_child = subtrees.t_below;
			subtrees.t_below->parent = t_negative;
			/*
			Node* t_negative;
			Node* t_positive = new Node();
			t_positive->parent = node;
			t_positive->cut = node->cut;
			t_positive->negative_child = subtrees.t_above->negative_child;
			subtrees.t_above->negative_child->parent = node;
			t_positive->positive_child = subtrees.t_above->positive_child;
			subtrees.t_above->positive_child->parent = t_positive;

			t_negative = subtrees.t_below;
			t_negative->parent = node;
			*/
			node->positive_child = t_positive;
			node->negative_child = t_negative;
			node->cut = cut;
		}
		else 
		{
			partition_priority(subtrees.t_below, cut);

			Node* t_positive = new Node();
			Node* t_negative;

			t_negative = subtrees.t_below->negative_child;
			t_negative->parent = node;

			t_positive->parent = node;
			t_positive->cut = node->cut;
			t_positive->positive_child = subtrees.t_above;
			subtrees.t_above->parent = t_positive;
			t_positive->negative_child = subtrees.t_below->positive_child;
			subtrees.t_below->positive_child->parent = t_positive;

			/*
			Node* t_positive;
			Node* t_negative = new Node();
			t_negative->parent = node;
			t_negative->cut = node->cut;
			t_negative->positive_child = subtrees.t_below->positive_child;
			subtrees.t_below->positive_child->parent = t_negative;
			t_negative->negative_child = subtrees.t_below->negative_child;
			subtrees.t_below->negative_child->parent = t_negative;

			t_positive = subtrees.t_above;
			t_positive->parent = node;*/

			node->negative_child = t_negative;
			node->positive_child = t_positive;
			node->cut = cut;
			
			/*
			vector<Node*> leaves_i_care_about = vector<Node*>();
			get_leaves_by_traversal(t_positive, leaves_i_care_about);
			dump_nodes("new_leaves.dat", leaves_i_care_about);*/
		}
	}

	if (subtrees.type == LAB) 
	{
		//std::cout << "\n\n PARTITION LAB \n\n";
		if (segment_intersects_region(subtrees.t_below, cut->segment) && segment_intersects_region(subtrees.t_left, cut->segment)) 
		{
			partition_priority(subtrees.t_below, cut);
			partition_priority(subtrees.t_left, cut);

			Node* t_positive = new Node();
			Node* t_positive_positive = new Node();
			Node* t_negative = new Node();

			t_negative->cut = node->cut;
			t_negative->negative_child = subtrees.t_left->negative_child;
			subtrees.t_left->negative_child->parent = t_negative;
			t_negative->positive_child = subtrees.t_below->negative_child;
			subtrees.t_below->negative_child->parent = t_negative;

			t_negative->parent = node;

			t_positive->cut = node->cut;
			t_positive->parent = node;
			t_positive->negative_child = subtrees.t_left->positive_child;
			subtrees.t_left->positive_child->parent = t_positive;
			t_positive->positive_child = t_positive_positive;

			t_positive_positive->cut = subtrees.t_above->parent->cut;
			t_positive_positive->parent = t_positive;
			t_positive_positive->positive_child = subtrees.t_above;
			subtrees.t_above->parent = t_positive_positive;
			t_positive_positive->negative_child = subtrees.t_below->positive_child;
			subtrees.t_below->positive_child->parent = t_positive_positive;

			v_merge_priority(t_negative);
			node->positive_child = t_positive;
			node->negative_child = t_negative;
			node->cut = cut;
		}
		else 
		{
			partition_priority(subtrees.t_left, cut);
			partition_priority(subtrees.t_above, cut);

			Node* t_positive = new Node();
			Node* t_negative = new Node();
			Node* t_negative_positive = new Node();

			t_positive->parent = node;
			t_positive->cut = node->cut;
			t_positive->negative_child = subtrees.t_left->positive_child;
			subtrees.t_left->positive_child->parent = t_positive;
			t_positive->positive_child = subtrees.t_above->positive_child;
			subtrees.t_above->positive_child->parent = t_positive;

			v_merge_priority(t_positive);

			t_negative_positive->cut = subtrees.t_below->parent->cut;
			t_negative_positive->negative_child = subtrees.t_below;
			subtrees.t_below->parent = t_negative_positive;
			t_negative_positive->positive_child = subtrees.t_above->negative_child;
			subtrees.t_above->negative_child->parent = t_negative_positive;

			t_negative->cut = node->cut;
			t_negative->parent = node;
			t_negative->negative_child = subtrees.t_left->negative_child;
			subtrees.t_left->negative_child->parent = t_negative;
			t_negative->positive_child = t_negative_positive;
			t_negative_positive->parent = t_negative;

			node->positive_child = t_positive;
			node->negative_child = t_negative;
			node->cut = cut;
		}
	}

	if (subtrees.type == RAB) 
	{
		//std::cout << "\n\n PARTITION RAB \n\n";
		if (segment_intersects_region(subtrees.t_right, cut->segment) && segment_intersects_region(subtrees.t_above, cut->segment)) 
		{
			partition_priority(subtrees.t_above, cut);
			partition_priority(subtrees.t_right, cut);

			Node* t_positive = new Node();
			Node* t_negative = new Node();
			Node* t_negative_negative = new Node();

			t_positive->cut = node->cut;
			t_positive->parent = node;
			t_positive->positive_child = subtrees.t_right->positive_child;
			subtrees.t_right->positive_child->parent = t_positive;
			t_positive->negative_child = subtrees.t_above->positive_child;
			subtrees.t_above->positive_child->parent = t_positive;

			t_negative_negative->cut = subtrees.t_below->parent->cut;
			t_negative_negative->positive_child = subtrees.t_above->negative_child;
			subtrees.t_above->negative_child->parent = t_negative_negative;
			t_negative_negative->negative_child = subtrees.t_below;
			subtrees.t_below->parent = t_negative_negative;

			t_negative->cut = node->cut;
			t_negative->parent = node;
			t_negative->negative_child = t_negative_negative;
			t_negative_negative->parent = t_negative;
			t_negative->positive_child = subtrees.t_right->negative_child;
			subtrees.t_right->negative_child->parent = t_negative;


			v_merge_priority(t_positive);
			node->positive_child = t_positive;
			node->negative_child = t_negative;
			node->cut = cut;
		}
		else 
		{
			partition_priority(subtrees.t_below, cut);
			partition_priority(subtrees.t_right, cut);

			Node* t_positive = new Node();
			Node* t_negative = new Node();
			Node* t_positive_negative = new Node();

			t_negative->cut = node->cut;
			t_negative->parent = node;
			t_negative->negative_child = subtrees.t_below->negative_child;
			subtrees.t_below->negative_child->parent = t_negative;
			t_negative->positive_child = subtrees.t_right->negative_child;
			subtrees.t_right->negative_child->parent = t_negative;

			t_positive_negative->cut = subtrees.t_above->parent->cut;
			t_positive_negative->positive_child = subtrees.t_above;
			subtrees.t_above->parent = t_positive_negative;
			t_positive_negative->negative_child = subtrees.t_below->positive_child;
			subtrees.t_below->positive_child->parent = t_positive_negative;

			t_positive->cut = node->cut;
			t_positive->parent = node;
			t_positive->negative_child = t_positive_negative;
			t_positive_negative->parent = t_positive;
			t_positive->positive_child = subtrees.t_right->positive_child;
			subtrees.t_right->positive_child->parent = t_positive;

			v_merge_priority(t_negative);
			node->positive_child = t_positive;
			node->negative_child = t_negative;
			node->cut = cut;
		}
	}

	if (subtrees.type == RLAB) 
	{
		//std::cout << "\n\n PARTITION RLAB \n\n";

		if (segment_intersects_region(subtrees.t_above, cut->segment)) 
		{

			//std::cout << "\n\n hello from RLAB top \n\n";
		
			Cut* vertical_cut_1 = node->cut;
			Cut* vertical_cut_2 = subtrees.t_right->parent->cut;
			Cut* segment_cut = subtrees.t_above->parent->cut;

			partition_priority(subtrees.t_left, cut);
			partition_priority(subtrees.t_above, cut);
			partition_priority(subtrees.t_right, cut);
			
			Node* t_positive = new Node();
			Node* t_negative = new Node();
			Node* t_positive_positive = new Node();
			Node* t_negative_positive = new Node();
			Node* t_negative_positive_negative = new Node();

			t_positive->parent = node;
			t_positive->cut = vertical_cut_1;
			t_positive->negative_child = subtrees.t_left->positive_child;
			subtrees.t_left->positive_child->parent = t_positive;
			t_positive->positive_child = t_positive_positive;
			t_positive_positive->parent = t_positive;

			t_positive_positive->cut = vertical_cut_2;
			t_positive_positive->negative_child = subtrees.t_above->positive_child;
			subtrees.t_above->positive_child->parent = t_positive_positive;
			t_positive_positive->positive_child = subtrees.t_right->positive_child;
			subtrees.t_right->positive_child->parent = t_positive_positive;

			t_negative->cut = vertical_cut_1;
			t_negative->parent = node;
			t_negative->negative_child = subtrees.t_left->negative_child;
			subtrees.t_left->negative_child->parent = t_negative;
			t_negative->positive_child = t_negative_positive;
			t_negative_positive->parent = t_negative;

			t_negative_positive->cut = vertical_cut_2;
			t_negative_positive->negative_child = t_negative_positive_negative;
			t_negative_positive_negative->parent = t_negative_positive;
			t_negative_positive_negative->cut = segment_cut;
			t_negative_positive_negative->positive_child = subtrees.t_above->negative_child;
			subtrees.t_above->negative_child->parent = t_negative_positive_negative;
			t_negative_positive_negative->negative_child = subtrees.t_below;
			subtrees.t_below->parent = t_negative_positive_negative;

			t_negative_positive->positive_child = subtrees.t_right->negative_child;
			subtrees.t_right->negative_child->parent = t_negative_positive;

			v_merge_priority(t_positive_positive);
			v_merge_priority(t_positive);
			node->positive_child = t_positive;
			node->negative_child = t_negative;
			node->cut = cut;

		
		}
		else 
		{
			Cut* vertical_cut_1 = node->cut;
			Cut* vertical_cut_2 = subtrees.t_right->parent->cut;
			Cut* segment_cut = subtrees.t_above->parent->cut;

			partition_priority(subtrees.t_below, cut);
			partition_priority(subtrees.t_left, cut);
			partition_priority(subtrees.t_right, cut);

			Node* t_negative = new Node();
			Node* t_negative_positive = new Node();
			Node* t_positive = new Node();
			Node* t_positive_positive = new Node();
			Node* t_positive_positive_negative = new Node();

			t_negative->cut = vertical_cut_1;
			t_negative->parent = node;
			t_negative->negative_child = subtrees.t_left->negative_child;
			subtrees.t_left->negative_child->parent = t_negative;
			t_negative->positive_child = t_negative_positive;
			t_negative_positive->parent = t_negative;
			t_negative_positive->cut = vertical_cut_2;
			t_negative_positive->negative_child = subtrees.t_below->negative_child;
			subtrees.t_below->negative_child->parent = t_negative_positive;
			t_negative_positive->positive_child = subtrees.t_right->negative_child;
			subtrees.t_right->negative_child->parent = t_negative_positive;

			t_positive->parent = node;
			t_positive->cut = vertical_cut_1;
			t_positive->negative_child = subtrees.t_left->positive_child;
			subtrees.t_left->positive_child->parent = t_positive;
			t_positive->positive_child = t_positive_positive;
			t_positive_positive->parent = t_positive;
			t_positive_positive->cut = vertical_cut_2;
			t_positive_positive->negative_child = t_positive_positive_negative;
			t_positive_positive_negative->cut = segment_cut;
			t_positive_positive_negative->parent = t_positive_positive;
			t_positive_positive_negative->negative_child = subtrees.t_below->positive_child;
			subtrees.t_below->positive_child->parent = t_positive_positive_negative;
			t_positive_positive_negative->positive_child = subtrees.t_above;
			subtrees.t_above->parent = t_positive_positive_negative;
			t_positive_positive->positive_child = subtrees.t_right->positive_child;
			subtrees.t_right->positive_child->parent = t_positive_positive;

			v_merge_priority(t_negative_positive);
			v_merge_priority(t_negative);
			node->negative_child = t_negative;
			node->positive_child = t_positive;
			node->cut = cut;
		}
	}

}

void Tree::v_merge_priority(Node* node) 
{
	if (EXPERIMENTS == 1)
		v_merge_calls++;

	if (node->cut == nullptr || node->cut->type == SEGMENT) 
	{
		std::cout << "\n Can't v_merge if the node isn't destroyed by a verticcal cut \n";
		exit(0);
	}

	//std::cout << "\n\n\n HELLO FROM V_MERGE \n\n\n";
	//std::cout << "priority: " << node->priority() << "\n";
	if (node->negative_child->is_leaf() && node->positive_child->is_leaf()) 
	{

		//std::cout << "\n\n\n V_MERGE: LEAF CASE \n\n\n";
		node->cut = nullptr;
		node->negative_child = nullptr;
		node->positive_child = nullptr;
		return;
	}

	Node* t_right = node->positive_child;
	Node* t_left = node->negative_child;

	if (t_right->priority() == t_left->priority()) 
	{
		//std::cout << "\n\n\n V_MERGE: equal priority \n\n\n";
		Subtrees left_subtrees = retrieve_subtrees(t_left);
		Subtrees right_subtrees = retrieve_subtrees(t_right);

		if (left_subtrees.type == LAB) 
		{
			Node* t_negative;
			Node* t_positive = new Node();

			Node* merge_above = new Node();
			Node* merge_below = new Node();

			Cut* vertical_cut = node->cut;
			Cut* new_node_cut = left_subtrees.t_left->parent->cut;

			merge_above->cut = vertical_cut;
			merge_below->cut = vertical_cut;


			t_negative = left_subtrees.t_left;
			t_negative->parent = node;

			t_positive->cut = t_right->cut;
			t_positive->parent = node;

			if (right_subtrees.type == RAB) 
			{
			//	std::cout << "\n L:LAB R: RAB\n";
				t_positive->positive_child = right_subtrees.t_right;
				right_subtrees.t_right->parent = t_positive;
				Node* t_positive_negative = new Node();
				t_positive->negative_child = t_positive_negative;
				t_positive_negative->parent = t_positive;
				t_positive_negative->cut = left_subtrees.t_above->parent->cut;
				t_positive_negative->positive_child = merge_above;
				merge_above->parent = t_positive_negative;
				t_positive_negative->negative_child = merge_below;
				merge_below->parent = t_positive_negative;

				merge_above->negative_child = left_subtrees.t_above;
				left_subtrees.t_above->parent = merge_above;
				merge_above->positive_child = right_subtrees.t_above;
				right_subtrees.t_above->parent = merge_above;

				merge_below->negative_child = left_subtrees.t_below;
				left_subtrees.t_below->parent = merge_below;
				merge_below->positive_child = right_subtrees.t_below;
				right_subtrees.t_below->parent = merge_below;

				node->negative_child = t_negative;
				node->positive_child = t_positive;
				node->cut = new_node_cut;

				v_merge_priority(merge_above);
				v_merge_priority(merge_below);
			}
			else if (right_subtrees.type == AB)
			{
				//std::cout << "\n L:LAB R: AB\n";
				t_positive->cut = left_subtrees.t_above->parent->cut;
				t_positive->negative_child = merge_below;
				merge_below->parent = t_positive;
				t_positive->positive_child = merge_above;
				merge_above->parent = t_positive;

				merge_above->negative_child = left_subtrees.t_above;
				left_subtrees.t_above->parent = merge_above;
				merge_above->positive_child = right_subtrees.t_above;
				right_subtrees.t_above->parent = merge_above;

				merge_below->negative_child = left_subtrees.t_below;
				left_subtrees.t_below->parent = merge_below;
				merge_below->positive_child = right_subtrees.t_below;
				right_subtrees.t_below->parent = merge_below;

				node->negative_child = t_negative;
				node->positive_child = t_positive;
				node->cut = new_node_cut;
				v_merge_priority(merge_above);
				v_merge_priority(merge_below);
			}
		}
		else if (left_subtrees.type == AB) 
		{
			Node* t_positive;
			Node* t_negative;

			if (right_subtrees.type == AB) 
			{
				//std::cout << "\n L:AB R: AB\n";
				Cut* new_node_cut = left_subtrees.t_above->parent->cut;
				Cut* vertical_cut = node->cut;

				Node* merge_above = new Node();
				Node* merge_below = new Node();

				merge_above->cut = vertical_cut;
				merge_below->cut = vertical_cut;

				merge_above->negative_child = left_subtrees.t_above;
				left_subtrees.t_above->parent = merge_above;
				merge_above->positive_child = right_subtrees.t_above;
				right_subtrees.t_above->parent = merge_above;

				merge_below->negative_child = left_subtrees.t_below;
				left_subtrees.t_below->parent = merge_below;
				merge_below->positive_child = right_subtrees.t_below;
				right_subtrees.t_below->parent = merge_below;

				v_merge_priority(merge_above);
				v_merge_priority(merge_below);
				t_positive = merge_above;
				t_negative = merge_below;

				t_positive->parent = node;
				t_negative->parent = node;

				node->cut = new_node_cut;
				node->positive_child = t_positive;
				node->negative_child = t_negative;
			}
			else if (right_subtrees.type == RAB) 
			{
				//std::cout << "\n L:AB R: RAB\n";
				Cut* new_node_cut = right_subtrees.t_right->parent->cut;
				Cut* vertical_cut = node->cut;

				t_positive = right_subtrees.t_right;
				t_positive->parent = node;
				t_negative = new Node();
				t_negative->cut = left_subtrees.t_above->parent->cut;
				//cout << *left_subtrees.t_above->parent->cut;

				Node* merge_above = new Node();
				Node* merge_below = new Node();

				t_negative->negative_child = merge_below;
				t_negative->positive_child = merge_above;
				t_negative->parent = node;

				merge_above->cut = vertical_cut;
				merge_below->cut = vertical_cut;

				merge_above->negative_child = left_subtrees.t_above;
				left_subtrees.t_above->parent = merge_above;
				merge_above->positive_child = right_subtrees.t_above;
				right_subtrees.t_above->parent = merge_above;

				merge_below->negative_child = left_subtrees.t_below;
				left_subtrees.t_below->parent = merge_below;
				merge_below->positive_child = right_subtrees.t_below;
				right_subtrees.t_below->parent = merge_below;
				merge_above->parent = t_negative;
				merge_below->parent = t_negative;

				v_merge_priority(merge_below);
				v_merge_priority(merge_above);

				node->cut = new_node_cut;
				node->negative_child = t_negative;
				node->positive_child = t_positive;
			}
		}
	}
	else if (t_left->priority() < t_right->priority()) 
	{
		// If the priority of the left node is smaller than the right node,
		// left_subtrees must contain a t_right subtree.
		Subtrees left_subtrees = retrieve_subtrees(t_left);
		Cut* new_node_cut = t_left->cut;
		Cut* vertical_cut = node->cut;

		Node* merge = new Node();
		merge->cut = node->cut;
		merge->negative_child = t_left->positive_child;
		t_left->positive_child->parent = merge;
		merge->positive_child = t_right;
		t_right->parent = merge;

		Node* t_negative;
		Node* t_positive;

		t_negative = t_left->negative_child;
		t_negative->parent = node;
		t_positive = merge;
		t_positive->parent = node;

		v_merge_priority(merge);

		node->cut = new_node_cut;
		node->negative_child = t_negative;
		node->positive_child = t_positive;


		/*
		std::cout << "\n\n\n V_MERGE: left smaller priority \n\n\n";

		if (t_right->is_leaf()) 
		{
			std::cout << "\n The right node is a leaf \n";
			if (t_right->cut != nullptr) 
			{
				std::cout << "\n that's a funny looking leaf \n";
			}
		}

		Subtrees left_subtrees = retrieve_subtrees(t_left);
		std::cout << "\n got subtrees \n";
		Cut* vertical_cut = node->cut;
		Cut* new_node_cut = t_left->cut;
		Node* merge = new Node();
		merge->cut = vertical_cut;

		if (left_subtrees.type == RAB) 
		{
			std::cout << "\n\n\n RAB \n\n\n";
			if (t_right->is_leaf()) 
			{
				std::cout << "\n\n\n right is a leaf \n\n\n";
				Node* t_negative = left_subtrees.t_right->parent->negative_child;
				t_negative->parent = node;
				Node* t_positive = merge;
				t_positive->parent = node;

				merge->negative_child = left_subtrees.t_right;
				left_subtrees.t_right->parent = merge;
				merge->positive_child = t_right;
				t_right->parent = merge;

				v_merge_priority(merge);
				node->negative_child = t_negative;
				node->positive_child = t_positive;
				node->cut = new_node_cut;

			}
			else 
			{
				std::cout << "\n\n\n right not leaf \n\n\n";
				Subtrees right_subtrees = retrieve_subtrees(t_right);

				Node* t_negative = left_subtrees.t_right->parent->negative_child;
				t_negative->parent = node;
				Node* t_positive = new Node();

				t_positive->cut = t_right->cut;
				t_positive->parent = node;
				t_positive->positive_child = t_right->positive_child;
				t_right->positive_child->parent = t_positive;
				t_positive->negative_child = merge;
				merge->parent = t_negative;

				merge->negative_child = left_subtrees.t_right;
				left_subtrees.t_right->parent = merge;
				merge->positive_child = right_subtrees.t_left;
				right_subtrees.t_left->parent = merge;

				v_merge_priority(merge);
				node->negative_child = t_negative;
				node->positive_child = t_positive;
				node->cut = new_node_cut;
			}
		}
		else if (left_subtrees.type == RLAB) 
		{
			std::cout << "\n\n\n RLAB \n\n\n";
			if (t_right->is_leaf()) 
			{
				std::cout << "\n\n\n right is a leaf \n\n\n";
				Node* t_negative = left_subtrees.t_left;
				t_negative->parent = node;
				Node* t_positive = new Node();
				t_positive->parent = node;
				t_positive->cut = left_subtrees.t_right->parent->cut;
				t_positive->negative_child = left_subtrees.t_above->parent;
				left_subtrees.t_above->parent->parent = t_positive;
				t_positive->positive_child = merge;
				merge->parent = t_positive;
				merge->negative_child = left_subtrees.t_right;
				left_subtrees.t_right->parent = merge;
				merge->positive_child = t_right;
				t_right->parent = merge;

				v_merge_priority(merge);
				node->positive_child = t_positive;
				node->negative_child = t_negative;
				node->cut = new_node_cut;
			}
			else 
			{
				std::cout << "\n\n\n right not leaf \n\n\n";
				Subtrees right_subtrees = retrieve_subtrees(t_right);
				std::cout << "\n\n retrieved subtrees \n\n";
				Node* t_negative = left_subtrees.t_left;
				t_negative->parent = node;
				left_subtrees.t_left->parent = t_negative;
				
				
				if (right_subtrees.type == AB || right_subtrees.type == RAB) 
				{
					std::cout << "\n\n AB or RAB\n\n";
					Node* t_positive = merge;
					t_positive->parent = node;
					merge->negative_child = t_left->positive_child;
					t_left->positive_child->parent = merge;
					merge->positive_child = t_right;
					t_right->parent = merge;

					v_merge_priority(merge);

					node->cut = new_node_cut;
					node->negative_child = t_negative;
					node->positive_child = t_positive;

				}
				else 
				{
					std::cout << "\n\n RAB or RLAB \n\n";
					Node* t_positive = new Node();
					t_positive->parent = node;
					t_positive->cut = left_subtrees.t_right->parent->cut;
					t_positive->negative_child = left_subtrees.t_above->parent;
					left_subtrees.t_above->parent->parent = t_positive;

					Node* t_positive_positive = new Node();
					t_positive_positive->cut = t_right->cut;
					t_positive_positive->negative_child = merge;
					merge->parent = t_positive_positive;
					t_positive_positive->positive_child = right_subtrees.t_right->parent->positive_child;
					right_subtrees.t_right->parent->positive_child->parent = t_positive_positive;

					merge->negative_child = left_subtrees.t_right;
					left_subtrees.t_right->parent = merge;
					merge->positive_child = right_subtrees.t_left;
					right_subtrees.t_left->parent = merge;

					v_merge_priority(merge);

					node->cut = new_node_cut;
					node->negative_child = t_negative;
					node->positive_child = t_positive;
				}
			}
		}
		else 
		{
			std::cout << "\n\n this is democracy manifest\n\n";
		}*/
	}
	else if (t_left->priority() > t_right->priority())
	{
		
		Cut* new_node_cut = t_right->cut;
		Cut* vertical_cut = node->cut;
		Subtrees right_subtrees = retrieve_subtrees(t_right);

		Node* t_positive = t_right->positive_child;
		t_positive->parent = node;

		Node* merge = new Node();
		merge->cut = vertical_cut;
		merge->negative_child = t_left;
		t_left->parent = merge;
		merge->positive_child = right_subtrees.t_left;
		right_subtrees.t_left->parent = merge;

		v_merge_priority(merge);

		Node* t_negative = merge;
		t_negative->parent = node;

		node->cut = new_node_cut;
		node->positive_child = t_positive;
		node->negative_child = t_negative;

		//std::cout << "\n\n\n V_MERGE: right smaller priority \n\n\n";
		/*
		std::cout << t_right->priority() << "\n";
		std::cout << t_left->priority() << "\n";

		Subtrees right_subtrees = retrieve_subtrees(t_right);

		Node* merge = new Node();

		if (right_subtrees.type == LAB || right_subtrees.type == RLAB)
		{
			std::cout << "\n\n\n V_MERGE: right smaller LAB \n\n\n";
			Cut* new_node_cut = t_right->cut;
			Cut* vertical_cut = node->cut;
			
			if (t_left->is_leaf()) 
			{	
				Node* t_positive = t_right->positive_child;
				t_positive->parent = node;

				merge->cut = vertical_cut;
				Node* t_negative = merge;
				t_negative->parent = node;
				merge->negative_child = t_left;
				t_left->parent = merge;
				merge->positive_child = right_subtrees.t_left;
				right_subtrees.t_left->parent = merge;

				v_merge_priority(merge);
				node->positive_child = t_positive;
				node->negative_child = t_negative;
				node->cut = new_node_cut;
			} 
			else 
			{
				Subtrees left_subtrees = retrieve_subtrees(t_left);
				Node* t_positive = t_right->positive_child;
				t_positive->parent = node;

				Node* t_negative = new Node();
				t_negative->parent = node;

				if (left_subtrees.type == RAB) 
				{
					t_negative->cut = left_subtrees.t_right->parent->cut;
					t_negative->negative_child = left_subtrees.t_right->parent->negative_child;
					t_negative->negative_child->parent = t_negative;

					merge->cut = vertical_cut;
					t_negative->positive_child = merge;
					merge->parent = t_negative;
					merge->negative_child = left_subtrees.t_right;
					left_subtrees.t_right->parent = merge;
					merge->positive_child = right_subtrees.t_left;
					right_subtrees.t_left->parent = merge;


					v_merge_priority(merge);

					node->positive_child = t_positive;
					node->negative_child = t_negative;
					node->cut = new_node_cut;
				}
				else if (left_subtrees.type == LAB || left_subtrees.type == RLAB) 
				{

					std::cout << "\n\n !!!!!!!! \n\n";
					t_negative->cut = left_subtrees.t_left->parent->cut;
					t_negative->negative_child = left_subtrees.t_left;
					left_subtrees.t_left->parent = t_negative;

					merge->cut = vertical_cut;
					t_negative->positive_child = merge;
					merge->parent = t_negative;

					merge->negative_child = left_subtrees.t_above->parent;
					left_subtrees.t_above->parent->parent = merge;
					merge->positive_child = right_subtrees.t_left;
					right_subtrees.t_left->parent = merge;

					v_merge_priority(merge);

					node->cut = new_node_cut;
					node->negative_child = t_negative;
					node->positive_child = t_positive;
				}
				else if (left_subtrees.type == AB)
				{

					t_negative = merge;
					merge->cut = vertical_cut;
					merge->negative_child = t_left;
					t_left->parent = merge;
					merge->positive_child = right_subtrees.t_left;
					right_subtrees.t_left->parent = merge;

					v_merge_priority(merge);

					node->cut = new_node_cut;
					node->negative_child = t_negative;
					node->positive_child = t_positive;
				}
				else 
				{
					std::cout << "\n oh dear \n";
				}
			}
		}
		else
		{
			std::cout << "\n\n\n oh dear... \n\n\n";
			if (right_subtrees.type == AB) 
			{
				std::cout << "\n\n hmmm, that's strange \n\n";
				if (t_left->is_leaf()) 
				{
					std::cout << "\n left is a leaf \n";
				}
				else
				{
					Subtrees left_subtrees = retrieve_subtrees(t_left);
					if (left_subtrees.t_right != nullptr) 
					{
						std::cout << "\n we have a right region \n";
					}
				}

			}
		}
		*/
	}
	else 
	{
		std::cout << "\n\nSOMETHING IS WRONG\n\n";
	}
}

void Tree::insert_into_node(Node* node, Segment* segment, vector<Cut*> cuts) 
{
	//cout << "\n" << segment->seg << "\n";
	
	if (cuts.size() == 0) 
	{
		std::cout << "\n there has to be at least one cut when insertinig into a node \n";
		exit(0);
	}


	if (cuts.size() == 1) 
	{
		//std::cout << "\n one cut \n";
		partition_priority(node, cuts[0]);
	}
	
	if (cuts.size() == 2) 
	{
		//std::cout << "\n two cuts \n";
		v_partition_priority(node, cuts[0]);
		if (segment_intersects_region(node->positive_child, segment)) 
		{
			partition_priority(node->positive_child, cuts[1]);
		}
		else 
		{
			partition_priority(node->negative_child, cuts[1]);
		}
	}

	if (cuts.size() == 3) 
	{
		//std::cout << "\n three cuts \n";
		v_partition_priority(node, cuts[0]);
		v_partition_priority(node->positive_child, cuts[1]);
		partition_priority(node->positive_child->negative_child, cuts[2]);
	}
	
	
	if (CHECKING >= 100)
		if (!is_valid(root)) 
		{
			std::cout << "\n dynamic tree is not valid \n";
			exit(0);
		}
}

void Tree::insert_with_priority(Segment* segment) 
{
	clock_t start;
	clock_t end;
	int intersection_count_before_insert;
	int intersection_count_after_insert;

	if (EXPERIMENTS == 1) 
	{
		intersection_count_before_insert = number_of_intersections;
		number_of_segments++;
		start = clock();
	}

	//cout << "\n insert \n";
	Cut* vertical_source = new Cut();
	vertical_source->segment = segment;
	vertical_source->type == VERTICAL_SOURCE;

	Cut* vertical_target = new Cut();
	vertical_target->segment = segment;
	vertical_target->type = VERTICAL_TARGET;

	Cut* segment_cut = new Cut();
	segment_cut->segment = segment;
	segment_cut->type = SEGMENT;

	vector<Node*> nodes = search_with_priority(segment);

	//dump_nodes("priority_search.dat", nodes);

	Cut* carry_over = nullptr;
	vector<Cut*> cuts = vector<Cut*>();

	for (int i = 0; i < nodes.size(); i++) 
	{
		Node* node = nodes[i];
		
		BoundingBox next_box = get_bounding_box(node);
		/*
		if (cuts.size() > 1) 
		{
			Line_2 line_1 = cuts[0]->get_defining_line();
			Line_2 line_2 = cuts[1]->get_defining_line();

			
			if (!line_2.has_on_negative_side(Point_2(line_1.x_at_y(0), 0))) 
			{
				std::cout << "\n\n cut assumption violated \n\n";
				exit(0);
			}
		}
		*/

		if (cuts.size() == 3 && cuts[1]->type == SEGMENT_INTERSECTION) 
		{
			carry_over = cuts[1];
		}
		else if (cuts.size() == 2 && cuts[0]->type == SEGMENT_INTERSECTION) 
		{
			BoundingBox last_box = get_bounding_box(nodes[i - 1]);
			//if (segment_intersects_region(nodes[i - 1]->negative_child, cuts[0]->segment)) 
			if (next_box.top->segment == cuts[0]->intersecting_segment && last_box.bottom == next_box.top) 
			{
				carry_over = cuts[0];
			}
			else if (next_box.bottom->segment == cuts[0]->intersecting_segment && last_box.top == next_box.bottom)
			{
				carry_over = cuts[0];
			}
			else 
			{
				/*
				cout << "\n HELLO \n";
				cout << *cuts[0];
				vector<Node*> test = vector<Node*>();
				test.push_back(nodes[i-1]);
				test.push_back(node);
				
				if (nodes[i - 1] == node) 
				{
					std::cout << "\n that is strange \n";
				}
				else 
				{
					cout << get_bounding_box(nodes[i - 1]).cuts_to_segments()[0] << "\n";
					cout << get_bounding_box(nodes[i - 1]).cuts_to_segments()[1] << "\n";
					cout << get_bounding_box(nodes[i - 1]).cuts_to_segments()[2] << "\n";
					cout << get_bounding_box(node).cuts_to_segments()[0] << "\n";
					cout << get_bounding_box(node).cuts_to_segments()[1] << "\n";
					cout << get_bounding_box(node).cuts_to_segments()[2] << "\n";
				}
				
				dump_nodes("blah.dat", test);*/
				carry_over = nullptr;
			}
		}
		else 
		{
			carry_over == nullptr;
		}

		if (i > 0) {
			/*vector<Node*> test = vector<Node*>();
			test.push_back(nodes[i - 1]);
			test.push_back(node);
			dump_nodes("blah.dat", test);*/
		}
		cuts = get_cuts(node, segment, carry_over, vertical_source, vertical_target, segment_cut);
		
		/*
		if (EXPERIMENTS == 1)
		{
			if (cuts.size() > 1)
			{
				if (cuts[1]->type == SEGMENT_INTERSECTION) 
				{
					number_of_intersections++;
				}
				else if (cuts.size() == 2 && cuts[0]->type == SEGMENT_INTERSECTION && cuts[1]->type == SEGMENT) 
				{
					BoundingBox bb = get_bounding_box(node);
					Line_2 line = cuts[0]->get_defining_line();
					Line_2 left_line = bb.left->get_defining_line();
					if (bb.type == BTL || bb.type == BRTL) 
					{
						Point_2 intersection_point = intersect_lines(Line_2(segment->seg), left_line);
						if (bb.top->get_defining_line().has_on_negative_side(intersection_point) && bb.bottom->get_defining_line().has_on_positive_side(intersection_point)) 
						{
							number_of_intersections++;
						}
					}
				}
			}
		}*/

		if (cuts.size() == 2) 
		{
			if (cuts[0]->type == SEGMENT) 
			{
				cout << "\n ERROR: the first cut of 2 can't be a segment cut \n";
				exit(0);
			}
		}
		if (cuts.size() == 3) 
		{
			if (cuts[0]->type == SEGMENT || cuts[1]->type == SEGMENT) 
			{
				cout << "\n ERROR: the first or second cut of 3 can't be a segment cut \n";
				exit(0);
			}

			if (cuts[2]->type != SEGMENT) 
			{
				cout << "\n this is also unexpected \n";
				exit(0);
			}
		}

		insert_into_node(node, segment, cuts);
	}

	if (EXPERIMENTS == 1 && number_of_segments % step_size == 0) 
	{
		intersection_count_after_insert = number_of_intersections;
		int intersection_delta = intersection_count_after_insert - intersection_count_before_insert;

		end = clock();
		dynamic_insert_stats << number_of_segments << "\t";
		dynamic_insert_stats << segment->priority << "\t";
		dynamic_insert_stats << nodes.size() << "\t";
		dynamic_insert_stats << size(root) << "\t";
		dynamic_insert_stats << depth(root) << "\t";
		dynamic_insert_stats << nodes_visited_in_search << "\t";
		dynamic_insert_stats << v_partition_calls << "\t";
		dynamic_insert_stats << partition_calls << "\t";
		dynamic_insert_stats << v_merge_calls << "\t";
		dynamic_insert_stats << intersection_delta <<"\t";
		dynamic_insert_stats << number_of_intersections << "\t";
		dynamic_insert_stats << (double)(end - start) / CLOCKS_PER_SEC << "\n";
		//dynamic_insert_stats.close();

		nodes_visited_in_search = 0;
		v_partition_calls = 0;
		partition_calls = 0;
		v_merge_calls = 0;
	}
}