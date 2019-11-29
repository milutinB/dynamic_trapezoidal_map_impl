#pragma once
#include "Tree.h";

#define CHECKING 0

void fix_segment(Segment& segment)
{
	if (segment.seg.source().x() > segment.seg.target().x())
	{
		segment.seg = Segment_2(
			segment.seg.target(),
			segment.seg.source()
		);
	}
}

bool segment_comparison(Segment seg_1, Segment seg_2) 
{
	return seg_1.priority < seg_2.priority;
}

bool experiment_with_few_segments(Tree* tree, Tree* naive_tree) 
{
	vector<Segment> segments = vector<Segment>();
	vector<Segment> other_segments = vector<Segment>();
	//srand(60);
	for (int i = 0; i < 3; i++)
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

		other_segments.push_back(segments.back());
		//segments.back().priority = i+1;
		segments.back().priority = i+1;

		other_segments.back().priority = i+1;

		//std::cout << segments.back().priority << "\n";
		fix_segment(segments.back());
		fix_segment(other_segments.back());
	}

	if (CHECKING)
		for (int i = 0; i < segments.size(); i++) 
		{
			for (int j = 0; j < i; j++) 
			{
				if (segments[i].priority == segments[j].priority) 
				{
					std::cout << "\n equal priorities \n";
					exit(0);
				}
			}
		}

	
	std::random_shuffle(segments.begin(), segments.end());

	for (int i = 0; i < segments.size(); i++)
	{
		tree->insert_with_priority(&segments[i]);
		std::cout << segments[i].priority << "\n";
		//vector<Node*> leaves = vector<Node*>();
		//tree->get_leaves_by_traversal(tree->root, leaves);
		//tree->dump_nodes("iteration_" + to_string(i) + ".dat", leaves);
		//tree->insert(&segments.at(i));
	}
	std::cout << "\n finished inserts \n";
	
	//std::sort(other_segments.begin(), other_segments.end(), segment_comparison);
	
	for (int i = 0; i < segments.size(); i++)
	{
		//std::cout << segments.at(i).priority << "\n";
		//tree->insert_with_priority(&segments.at(i));
		naive_tree->insert(&other_segments.at(i));
	}


	if (tree->is_equal(naive_tree))
	{
		std::cout << "\n Trees look the same!! \n";
		return true;
	}
	else
	{
		std::cout << "\n trees look a bit different... \n";
		vector<Node*> leaves = vector<Node*>();
		naive_tree->get_nodes_from_traversal(naive_tree->root, leaves);
		naive_tree->dump_nodes("naive.dat", leaves);

		vector<Node*> n_leaves = vector<Node*>();
		tree->get_nodes_from_traversal(tree->root, n_leaves);
		tree->dump_nodes("partition_test.dat", n_leaves);

		for (auto s : segments) 
		{
			std::cout << s.seg << "\n";
		}

		return false;
	}
}

void partition_test(Tree* tree) 
{

	/*
	Segment_2 seg_1 = Segment_2(
		Point_2(30, 60),
		Point_2(70, 60)
	);

	Segment segment_1 = Segment();
	segment_1.seg = seg_1;
	segment_1.priority = 1;

	Segment_2 seg_2 = Segment_2(
		Point_2(20, 40),
		Point_2(80, 40)
	);

	Segment segment_2 = Segment();
	segment_2.seg = seg_2;
	segment_2.priority = 3;

	Segment_2 seg_3 = Segment_2(
		Point_2(25, 50),
		Point_2(75, 30)
	);

	Segment segment_3 = Segment();
	segment_3.seg = seg_3;
	segment_3.priority = 2;


	Tree* naive_tree = new Tree();
	naive_tree->root = new Node();
	naive_tree->enclosing_box = tree->enclosing_box;
	
	naive_tree->insert(&segment_1);
	naive_tree->insert(&segment_3);
	naive_tree->insert(&segment_2);

	tree->insert_with_priority(&segment_1);
	tree->insert_with_priority(&segment_2);
	tree->insert_with_priority(&segment_3);

	
	vector<Node*> leaves = vector<Node*>();
	naive_tree->get_leaves_by_traversal(naive_tree->root, leaves);
	naive_tree->dump_nodes("naive.dat", leaves);

	if (tree->is_equal(naive_tree))
	{
		std::cout << "\n Trees look the same!! \n";
	}
	else
	{
		std::cout << "\n trees look a bit different... \n";
	}*/

	BoundingBox enclosing_box = tree->enclosing_box;

	for (int i = 0; i < 5000; i++) 
	{
		Tree* dynamic_tree = new Tree();
		dynamic_tree->root = new Node();
		dynamic_tree->enclosing_box = enclosing_box;

		Tree* static_tree = new Tree();
		static_tree->root = new Node();
		static_tree->enclosing_box = enclosing_box;

		if (!experiment_with_few_segments(dynamic_tree, static_tree)) 
		{
			exit(0);
		}
	}
}