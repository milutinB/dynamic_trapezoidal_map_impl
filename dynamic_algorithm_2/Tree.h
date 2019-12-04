#pragma once

#include "SubtreeStruct.h"
#include "BoundingBox.h"
#include <iostream>
#include <stack>
#define CHECKING 1;

//using namespace std;

enum NodeDestructionPattern { VVE, VE, E, NO_INTERSECTION };


struct Tree
{
	vector<Node*> nodes;
	Node* root;
	BoundingBox enclosing_box;
	vector<Cut> leaf_destruction_cuts;

	int seg_count = 0;

	int v_merge_calls = 0;
	int v_partition_calls = 0;
	int partition_calls = 0;

	// Returns the bounding box of the given node.
	// The bounding box is a struct containing the cuts
	// that define the region of the node.
	BoundingBox get_bounding_box(Node* node);


	void get_nodes_from_traversal(Node* node, vector<Node*> &nodes);

	// Checks if the query point is inside the region
	// of the given node
	bool region_contains_point(Node* node, Point_2 query_point);

	bool region_contains_point_strict(Node* node, Point_2 query_point);

	// Checks if the given segment intersects the region
	// of the given node.
	bool segment_intersects_region(Node* node, Segment* segment);

	// Get depth of the tree

	int depth(Node* node);

	// Find all leaves whose regions intersect segment
	vector<Node*> search(Segment* segment);

	vector<Node*> v_partition(Node* node, Cut* cut);

	vector<Node*> partition(Node* node, Segment* segment);

	void v_partition_naive(Node* node, Cut* vetical_cut);

	void partition_naive(Node* node, Segment* segment);

	void dump_nodes(string file_name, vector<Node*> nodes);

	vector<Node*> get_leaves();

	CutType determine_vertical_cut_type(Node* node, Segment* segment);

	// Updates the search tree due to the
	// addition of the new segment
	void insert(Segment* segment);

	void naive_insert(Segment* segment);

	// Compares two traoezoidal search trees
	bool is_equal(Tree* other_tree);

	// checks if tree is valid
	bool is_valid(Node* node);

	vector<Node*> naive_search_with_priority(Segment* segment);


	Node* deepest_node_containing_segment(Node* node, Segment* segment);

	// Returns a vector of nodes corresponding to the roots of
	// all subtrees affected by the insertion of the input segment.
	// The nodes are ordered left to right in the order in which they
	// intersect the segment.
	vector<Node*> search_with_priority(Segment* segment);

	int count_intersections();

	// Returns the roots of up to four subtrees
	// with the next higher priority
	Subtrees retrieve_subtrees(Node* node);

	// Retrieves the cuts induced by the intersection of the segment 
	// with the region corresponding to the node.
	// The carry_on cut may be a nullptr or an intersection cut.
	// In the former case, it is the first cut pushed onto the output vector.
	vector<Cut*> get_cuts(Node* node, Segment* segment, Cut* carry_over, Cut* vertical_source, Cut* vertical_target, Cut* segment_cut);


	void v_partition_priority(Node* node, Cut* cut);


	// The node passed to this must be destroyed by a vertical cut.
	// Moreover, the point defining the cut must not be in the region of the node.
	void v_merge_priority(Node* node);

	void partition_priority(Node* node, Cut* cut);

	void insert_with_priority(Segment* segment);

	void insert_into_node(Node* node, Segment* segment, vector<Cut*> cuts);

	void get_leaves_by_traversal(Node* root, vector<Node*> &leaves)
	{
		if (root->is_leaf()) 
		{
			leaves.push_back(root);
		}
		else 
		{
			get_leaves_by_traversal(root->positive_child, leaves);
			get_leaves_by_traversal(root->negative_child, leaves);
		}
	}

	int get_node_depth(Node* node, int depth) 
	{
		//std::cout << "hi";
		if (node == root) 
		{
			return depth;
		}
		else 
		{
			return get_node_depth(node->parent, depth+1);
		}
	}
};