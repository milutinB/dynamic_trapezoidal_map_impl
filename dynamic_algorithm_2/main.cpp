#pragma once

#include <iostream>;
#include <fstream>;
#include "Tree.h";
#include "Cut.h";
#include "BoundingBox.h";
#include "Segment.h";
#include "Node.h";
#include <cmath>
#include <time.h>
#include "retrieve_subtree_test.h"
#include "destruction_pattern_test.h"
//#include "v_partition_test.h";
//#include "partition_test.h";
//#include "v_merge_test.h"
#include "experiment.h";
#include "common_endpoint_test.h";

using namespace std;

int main(int argc, char* argv[]) 
{
	Tree tree = Tree();
	tree.enclosing_box = BoundingBox();
	
	Tree other_tree = Tree();

	Segment_2 top_seg = Segment_2(
		Point_2(0, 100),
		Point_2(100, 100)
	);
	Segment top_segment = Segment();
	top_segment.seg = top_seg;
	Cut top_cut = Cut();
	top_cut.segment = &top_segment;
	top_cut.type = SEGMENT;


	Segment_2 right_seg = Segment_2(
		Point_2(100, 50),
		Point_2(110, 50)
	);
	Segment right_segment = Segment();
	right_segment.seg = right_seg;
	Cut right_cut = Cut();
	right_cut.segment = &right_segment;
	right_cut.type = VERTICAL_SOURCE;


	Segment_2 left_seg = Segment_2(
		Point_2(-1, 50),
		Point_2(0, 500)
	);
	Segment left_segment = Segment();
	left_segment.seg = left_seg;
	Cut left_cut = Cut();
	left_cut.segment = &left_segment;
	left_cut.type = VERTICAL_TARGET;

	Segment_2 bottom_seg = Segment_2(
		Point_2(0, 0),
		Point_2(100, 0)
	);
	Segment bottom_segment = Segment();
	bottom_segment.seg = bottom_seg;
	Cut bottom_cut = Cut();
	bottom_cut.segment = &bottom_segment;
	bottom_cut.type = SEGMENT;


	tree.enclosing_box.top = &top_cut;
	tree.enclosing_box.bottom = &bottom_cut;
	tree.enclosing_box.left = &left_cut;
	tree.enclosing_box.right = &right_cut;
	tree.root = new Node();
	
	/*
	other_tree.enclosing_box = tree.enclosing_box;
	
	Segment_2 seg_1 = Segment_2(
		Point_2(45.0868, 0.100101),
		Point_2(64.6992, 15.4643)
	);
	Segment segment_1 = Segment();
	segment_1.seg = seg_1;

	Segment_2 seg_2 = Segment_2(
		Point_2(38.3898, 46.8007),
		Point_2(71.677, 28.0233)
	);
	Segment segment_2 = Segment();
	segment_2.seg = seg_2;

	Segment_2 seg_3 = Segment_2(
		Point_2(59.7284, 65.8272),
		Point_2(68.7155, 13.9286)
	);
	Segment segment_3 = Segment();
	segment_3.seg = seg_3;

	Segment_2 seg_4 = Segment_2(
		Point_2(1.19877, 24.3196),
		Point_2(41.0828, 56.8401)
	);
	Segment segment_4 = Segment();
	segment_4.seg = seg_4;

	Segment_2 seg_5 = Segment_2(
		Point_2(13.2719, 11.785),
		Point_2(29.1562, 7.31233)
	);
	Segment segment_5 = Segment();
	segment_5.seg = seg_5;

	Segment_2 seg_6 = Segment_2(
		Point_2(0.373547, 9.52666),
		Point_2(35.6554, 79.082)
	);
	Segment segment_6 = Segment();
	segment_6.seg = seg_6;

	Segment_2 seg_7 = Segment_2(
		Point_2(70, 11),
		Point_2(93, 71)
	);
	Segment segment_7 = Segment();
	segment_7.seg = seg_7;


	Node node_A = Node();
	tree.root = &node_A;
	tree.nodes = vector<Node*>();
	tree.nodes.push_back(&node_A);

	other_tree.root = new Node();
	other_tree.nodes = vector<Node*>();
	other_tree.nodes.push_back(other_tree.root);

	BoundingBox bb = tree.get_bounding_box(&node_A);
	vector<Segment_2> segs = bb.get_top_and_bottom_segments();*/

	if (argc != 5) 
	{
		std::cout << "\nError: incorrect number of parameters given\n";
		exit(0);
	}

	int experiment_size = std::atoi(argv[1]);
	std::string type = (std::string)argv[2];
	int seed = std::atoi(argv[3]);
	int step_size = std::atoi(argv[4]);

	experiment(&tree, experiment_size, type, seed, step_size);
}