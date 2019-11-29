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
#include "partition_test.h";
//#include "v_merge_test.h"


using namespace std;


/*
void fix_segment(Segment& segment) 
{
	if (segment.seg.source().x() > segment.seg.target().x()) 
	{
		segment.seg = Segment_2(
			segment.seg.target(),
			segment.seg.source()
		);
	}
}*/

int main() 
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
	vector<Segment_2> segs = bb.get_top_and_bottom_segments();

	//cout << segs[0];
	//cout << segs[1];

	//retrieve_subtree_test(&tree);
	//destruction_pattern_test(&tree);
	
	//v_partition_test(&tree);
	partition_test(&tree);
	//v_merge_test(&tree);


	/*
	vector<Segment> segments = vector<Segment>();
	srand(1337);
	for (int i = 0; i < 32; ++i) 
	{
		segments.push_back(Segment());
		
		double x = 10 + ((double)rand() / RAND_MAX) * 80;
		double y = 10 + ((double)rand() / RAND_MAX) * 80;

		double x_ = ((double)rand() / RAND_MAX) * 80;
		double y_ = ((double)rand() / RAND_MAX) * 80;


		segments.back().seg = Segment_2(
			Point_2(x, y),
			//Point_2(x + 5 * x_ / (pow( pow(x_, 2) + pow(y_, 2),0.5)), y + 5 * y_ / (pow(pow(x_, 2) + pow(y_, 2), 0.5)))
			Point_2(x_, y_)
		);

		segments.back().priority = i+1;

		fix_segment(segments.back());
		//cout << segments.back() << "\n";
	}
	

	cout << segments.size();
	
	clock_t start = clock();
	for (int i = 0; i < segments.size(); ++i) 
	{
		tree.insert(&segments.at(i));
		//other_tree.naive_insert(&segments.at(i));
		//if (i > 1)
		//	other_tree.insert(&segments.at(i));
	}
	clock_t end = clock();



	
	cout << "n: " << segments.size() << "\n";
	cout << "number of nodes " << tree.nodes.size() << "\n";
	cout <<  "size / nlogn: " << tree.nodes.size() / (segments.size() * (log(segments.size()) / log(2))) << "\n";
	cout << "depth: " << tree.depth(tree.root) << "\n";
	cout << "opt depth: " << log(segments.size()) / log(2) << "\n";
	cout << (double)(end - start) / CLOCKS_PER_SEC;*/

//	Segment new_segment = Segment();

//	new_segment.seg = Segment_2(
//		Point_2(5, 95),
//		Point_2(95, 5)
//	);

	//new_segment.priority = 20;

	//vector<Node*> roots_of_subtrees = tree.search_with_priority(&new_segment);
	
	//tree.dump_nodes("roots_of_subtrees.dat", roots_of_subtrees);

	/*
	tree.insert(&segment_1);
	tree.insert(&segment_2);
	tree.insert(&segment_3);
	tree.insert(&segment_4);
	tree.insert(&segment_5);
	tree.insert(&segment_6);
	tree.insert(&segment_7);
	*/

	/*
	cout << "\n\n\n\n\n\n\n................";

	tree.dump_nodes("test.dat", tree.get_leaves());

	cout << tree.nodes.size();
	cout << "\n";
	cout << tree.get_leaves().size();
	cout << "\n";
	for (auto s : segments) 
	{
		cout << s;
		cout << "\n";
	}*/


	//cout << "\n intersection count \n";

	//cout << tree.count_intersections();

	/*
	if (tree.is_equal(&other_tree)) 
	{
		cout << "\n TREES EQUAL";
	}
	else 
	{
		cout << "\nTREES NOT EQUAL\n";
	}*/

	/*
	fstream file;


	
	file.open("test.dat", 'w');

	for (auto node : tree.nodes) 
	{
		if (node->is_leaf()) 
		{
			BoundingBox bb = tree.get_bounding_box(node);
			//cout << bb;
			vector<Segment_2> segs = bb.cuts_to_segments();

			for (auto s : segs) 
			{
				file << s;
				file << "\n";
			}
			file << "\n";
		}
	}

	file.close();*/

	


	
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