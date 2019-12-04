#pragma once
#include <random>
#include "Tree.h";

# define PI           3.14159265358979323846

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

vector<Segment> generate_segments_with_few_intersections(int input_size, int seed) 
{
	vector<Segment> segments = vector<Segment>();
	std::mt19937_64 generator(seed);
	std::uniform_real_distribution<double> uniform_dist(6, 94);
	std::uniform_real_distribution<double> uniform_dist_pi(0, 2*PI);
	std::uniform_real_distribution<double> uniform_dist_scale(1, 5);
	for (int i = 0; i < input_size; i++) 
	{
		double x = uniform_dist(generator);
		double y = uniform_dist(generator);
		double theta = uniform_dist_pi(generator);
		double scale_factor = uniform_dist_scale(generator);

		Point_2 point_1 = Point_2(x, y);
		Point_2 point_2 = Point_2(scale_factor*std::cos(theta) + x, scale_factor*std::sin(theta) + y);

		segments.push_back(Segment());
		segments.back().seg = Segment_2(
			point_1,
			point_2
		);

		segments.back().priority = i + 1;
		fix_segment(segments.back());
	}

	return segments;
}

vector<Segment> generate_segments(int input_size, int seed) 
{
	vector<Segment> segments = vector<Segment>();
	std::mt19937_64 generator(seed);
	std::uniform_real_distribution<double> uniform_dist(1, 99);

	for (int i = 0; i < input_size; i++) 
	{
		double x = uniform_dist(generator);
		double y = uniform_dist(generator);
		double x_ = uniform_dist(generator);
		double y_ = uniform_dist(generator);
		//double x = 10 + ((double)rand() / RAND_MAX) * 80;
		//double y = 10 + ((double)rand() / RAND_MAX) * 80;
		//double x_ = 10 + ((double)rand() / RAND_MAX) * 80;
		//double y_ = 10 + ((double)rand() / RAND_MAX) * 80;
		
		segments.push_back(Segment());
		segments.back().seg = Segment_2(
			Point_2(x, y),
			Point_2(x_, y_)
		);
		segments.back().priority = i + 1;
		
		fix_segment(segments.back());
	}

	return segments;
}

vector<Segment> generate_segments_with_no_intersections(int input_size, int seed)
{
	vector<Segment> segments = vector<Segment>();
	std::mt19937_64 generator(seed);
	std::uniform_real_distribution<double> uniform_dist(1, 99);

	for (int i = 0; i < input_size; i++)
	{
		double x = uniform_dist(generator);
		double y = uniform_dist(generator);
		double x_ = uniform_dist(generator);

		segments.push_back(Segment());
		segments.back().seg = Segment_2(
			Point_2(x, y),
			Point_2(x_, y)
		);
		segments.back().priority = i + 1;

		fix_segment(segments.back());
	}

	return segments;
}

bool is_valid_input(vector<Segment> segments) 
{

	vector<double> x_coords = vector<double>();
	int intersection_count = 0;
	for (int i = 0; i < segments.size(); i++)
	{
		x_coords.push_back(to_double(segments[i].seg.source().x()));
		x_coords.push_back(to_double(segments[i].seg.target().x()));

		
		for (int j = i + 1; j < segments.size(); j++)
		{
			Segment_2 seg_1 = segments[i].seg;
			Segment_2 seg_2 = segments[j].seg;
			if (check_for_segment_intersection(seg_1, seg_2))
			{
				intersection_count++;
				x_coords.push_back(to_double(intersect_segments(seg_1, seg_2).x()));
			}
		}
	}

	std::sort(x_coords.begin(), x_coords.end());

	for (int i = 1; i < x_coords.size(); ++i)
	{
		if (x_coords[i - 1] == x_coords[i])
		{
			std::cout << "\n skipping due to equal x \n";
			return false;
		}
	}

	std::cout << "\n number of intersection: " << intersection_count << "\n";
	return true;
}

void experiment(Tree* tree, int input_size, std::string type, int seed) 
{
	
	std::cout << "\n hello from experiment \n";
	vector<Segment> segments;

	if (type == "many") 
	{
		segments = generate_segments(input_size, seed);
		while (!is_valid_input(segments))
		{
			segments = generate_segments(input_size, seed);
		}

	}
	else if (type == "few") 
	{
		segments = generate_segments_with_few_intersections(input_size, seed);
		while (!is_valid_input(segments))
		{
			segments = generate_segments_with_few_intersections(input_size, seed);
		}

	}
	else if (type == "none") 
	{
		segments = generate_segments_with_no_intersections(input_size, seed);
		while (!is_valid_input(segments)) 
		{
			segments = generate_segments_with_no_intersections(input_size, seed);
		}
	}
	else 
	{
		std::cout << "\n error: invalid experiment type \n";
		exit(0);
	}


	vector<Segment> other_segments = vector<Segment>();

	for (int i = 0; i < segments.size(); ++i) 
	{
		other_segments.push_back(segments[i]);
	}

	std::cout << "\n finished validating input \n";

	Tree* naive_tree = new Tree();
	naive_tree->enclosing_box = tree->enclosing_box;
	naive_tree->root = new Node();

	naive_tree->dynamic_insert_stats.open("static_insert_stats.dat", 'w');
	naive_tree->dynamic_insert_stats << "# static inserts of 1000 segments, segments with random endpoints, seed=1337\n";
	naive_tree->dynamic_insert_stats << "# segments" << "\t";
	naive_tree->dynamic_insert_stats << "priority" << "\t";
	naive_tree->dynamic_insert_stats << "roots of subtrees" << "\t";
	naive_tree->dynamic_insert_stats << "size" << "\t";
	naive_tree->dynamic_insert_stats << "depth" << "\t";
	naive_tree->dynamic_insert_stats << "# visited" << "\t";
	naive_tree->dynamic_insert_stats << "v-part" << "\t";
	naive_tree->dynamic_insert_stats << "part" << "\t";
	naive_tree->dynamic_insert_stats << "v_merge" << "\t";
	naive_tree->dynamic_insert_stats << "# intersections" << "\t";
	naive_tree->dynamic_insert_stats << "cumulative intersections" << "\t";
	naive_tree->dynamic_insert_stats << "insert execution time" << "\n";

	for (int i = 0; i < segments.size(); i++)
	{
		naive_tree->insert(&other_segments[i]);
	}

	naive_tree->dynamic_insert_stats.close();

	std::cout << "\n finished static inserts \n";

	std::random_shuffle(segments.begin(), segments.end());

	tree->dynamic_insert_stats.open("dynamic_insert_stats.dat", 'w');
	tree->dynamic_insert_stats << "# dynamic inserts of 1000 segments, segments with random endpoints, seed=1337\n";
	tree->dynamic_insert_stats << "# segments" << "\t";
	tree->dynamic_insert_stats << "priority" << "\t";
	tree->dynamic_insert_stats << "roots of subtrees" << "\t";
	tree->dynamic_insert_stats << "size" << "\t";
	tree->dynamic_insert_stats << "depth" << "\t";
	tree->dynamic_insert_stats << "# visited" << "\t";
	tree->dynamic_insert_stats << "v-part" << "\t";
	tree->dynamic_insert_stats << "part" << "\t";
	tree->dynamic_insert_stats << "v_merge" << "\t";
	tree->dynamic_insert_stats << "# intersections" << "\t";
	tree->dynamic_insert_stats << "cumulative intersections" << "\t";
	tree->dynamic_insert_stats << "insert execution time" << "\n";

	for (int i = 0; i < segments.size(); i++) 
	{
		tree->insert_with_priority(&segments[i]);
	}

	tree->dynamic_insert_stats.close();

	std::cout << "\n finished dynamic inserts \n";
	
	if (tree->is_equal(naive_tree)) 
	{
		std::cout << "\n trees look the same \n";
	}
	else 
	{
		std::cout << "\n trees look a little different \n";
	}

	vector<Node*> dynamic_leaves = vector<Node*>();
	tree->get_leaves_by_traversal(tree->root, dynamic_leaves);
	tree->dump_nodes("dynamic_leaves.dat", dynamic_leaves);

	vector<Node*> static_leaves = vector<Node*>();
	naive_tree->get_leaves_by_traversal(naive_tree->root, static_leaves);
	naive_tree->dump_nodes("static_leaves.dat", static_leaves);

	
}