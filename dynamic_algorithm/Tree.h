#pragma once

#include "Node.h"
#include "BoundingBox.h"
#include <iostream>

using namespace std;

enum LeafDestructionPattern { VVE, VE, E };

struct Tree 
{
	vector<Node*> nodes;
	Node* root;
	BoundingBox enclosing_box;
	vector<Cut> leaf_destruction_cuts;

	// Returns the bounding box of the given node.
	// The bounding box is a struct containing the cuts
	// that define the region of the node.
	BoundingBox get_bounding_box(Node* node);


	// Checks if the query point is inside the region
	// of the given node
	bool region_contains_point(Node* node, Point_2 query_point);

	bool region_contains_point_strict(Node* node, Point_2 query_point);

	// Checks if the given segment intersects the region
	// of the given node.
	bool segment_intersects_region(Node* node, Segment* segment);


	// Find all leaves whose regions intersect segment
	vector<Node*> search(Segment* segment);

	void v_partition(Node* node, Cut& vetical_cut);

	void partition(Node* node, Segment* segment);

	CutType determine_vertical_cut_type(Node* node, Segment* segment);

	// Updates the search tree due to the
	// addition of the new segment
        //
        // 1. Locate all sub-trees whose roots have higher prio and intersect segment. Store them left-to-right.

        // 2. For each such node u left-to-right
        
        //      * Determine LeafDestructionPattern
        //      * Push the newly created Cuts to leaf_destruction_cuts
        //      * do the vPartition calls on u in respect to the pattern:
        // 
        //      * Copy over the contents of u in a temporary node v, make u a leaf by clearing its content.
        //      * Cut the leaf: assign it to the vertical cut and allocate new child nodes;
        //      * vPart(v,cut, u-,u+).
        //      * If the resulting region is not properly intersected by s, we are done.
        //
        //      * Copy over the contents of u in a temporary node v, make u a leaf by clearing its content.
        //      * Cut the leaf: assign it to the vertical cut and allocate new child nodes;
        //      * vPart(v,cut, u-,u+).
        //      * If the resulting region is not properly intersected by s, we are done.
        //      
        //      * Partition

	void insert(Segment* segment);
};
