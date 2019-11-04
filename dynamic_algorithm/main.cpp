#pragma once

#include <iostream>;
#include "Tree.h";
#include "Cut.h";
#include "BoundingBox.h";
#include "Segment.h";
#include "Node.h";

using namespace std;

int main() 
{


	Tree tree = Tree();
	tree.enclosing_box = BoundingBox();

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
		Point_2(-10, 50),
		Point_2(0, 50)
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


	
	Segment_2 seg_1 = Segment_2(
		Point_2(25, 50),
		Point_2(75, 50)
	);
	Segment segment_1 = Segment();
	segment_1.seg = seg_1;

	Segment_2 seg_2 = Segment_2(
		Point_2(30, 30),
		Point_2(60, 60)
	);
	Segment segment_2 = Segment();
	segment_2.seg = seg_2;

	Node node_A = Node();
	tree.root = &node_A;
	tree.nodes = vector<Node*>();
	tree.nodes.push_back(&node_A);

	BoundingBox bb = tree.get_bounding_box(&node_A);
	vector<Segment_2> segs = bb.get_top_and_bottom_segments();

	//cout << segs[0];
	//cout << segs[1];


	/*	vector<Segment> segments = vector<Segment>();

	for (int i = 0; i < 4; ++i) 
	{
		segments.push_back(Segment());

		segments.back().seg = Segment_2(
			Point_2(((double)rand() / RAND_MAX) * 80, ((double)rand() / RAND_MAX) * 80),
			Point_2(((double)rand() / RAND_MAX) * 80, ((double)rand() / RAND_MAX) * 80)
		);
		cout << segments.back() << "\n";

	}
	
	cout << segments.size();

	
	for (auto s : segments) 
	{
		tree.insert(&s);
	}*/
	tree.insert(&segment_1);
	tree.insert(&segment_2);

	cout << "\n";
	for (auto node : tree.nodes) 
	{
		if (node->is_leaf()) 
		{
			BoundingBox bb = tree.get_bounding_box(node);
			//cout << bb;
			vector<Segment_2> segs = bb.cuts_to_segments();

			for (auto s : segs) 
			{
				cout << s;
				cout << "\n";
			}
			cout << "\n";
		}
	}

	


	
	/*
	Cut cut_A = Cut();
	cut_A.segment = &segment_1;
	cut_A.type = VERTICAL_SOURCE;
	node_A.cut = cut_A;
	

	
	Node node_B = Node();
	node_B.parent = &node_A;
	node_A.negative_child = &node_B;

	Node node_C = Node();
	Cut cut_C = Cut();
	cut_C.segment = &segment_1;
	cut_C.type = VERTICAL_TARGET;
	node_C.cut = cut_C;
	node_C.parent = &node_A;
	node_A.positive_child = &node_C;


	Node node_D = Node();
	node_D.parent = &node_C;
	node_C.negative_child = &node_D;
	Cut cut_D = Cut();
	cut_D.segment = &segment_1;
	cut_D.type = SEGMENT;
	node_D.cut = cut_D;

	Node node_E = Node();
	node_E.parent = &node_C;
	node_C.positive_child = &node_C;
	
	Node node_F = Node();
	node_F.parent = &node_D;
	node_D.positive_child = &node_F;

	Node node_G = Node();
	node_G.parent = &node_D;
	node_D.negative_child = &node_G;


	vector<Node*> nodes = vector<Node*>();

	tree.root = &node_A;

	nodes.push_back(&node_A);
	nodes.push_back(&node_B);
	nodes.push_back(&node_C);
	nodes.push_back(&node_D);
	nodes.push_back(&node_E);
	nodes.push_back(&node_F);
	nodes.push_back(&node_G);

	Segment_2 seg_2 = Segment_2(
		Point_2(10, 90),
		Point_2(90, 90)
	);

	tree.nodes = nodes;

	Segment segment_2 = Segment();
	segment_2.seg = seg_2;

	vector<Node*> leaves = tree.search(&segment_2);

	cout << leaves.size();

	
	BoundingBox bb = tree.get_bounding_box(&node_G);
	cout << bb;

	vector<Segment_2> segs = bb.cuts_to_segments();
	cout << "\n";
	for (auto s : segs) 
	{
		cout << s;
		cout << "\n";
	}

	if (bb.contains_point(Point_2(50, 25))) 
	{
		cout << "yes";
	}
	else 
	{
		cout << "no";
	}*/

}