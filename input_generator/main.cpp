#include "input_utils.h";



vector<Segment_3> rect_to_seg(vector<Rectangle> rects)
{
	vector<Segment_3> output = vector<Segment_3>();

	for (auto r : rects)
	{
		Point_3 p_1 = Point_3(r.c_x, r.c_y, r.c_z);
		Point_3 p_2 = Point_3(r.c_x, r.c_y + r.w * r.d_y, r.c_z + r.w * r.d_z);
		Point_3 p_3 = Point_3(r.c_x + r.l, r.c_y, r.c_z);
		Point_3 p_4 = Point_3(r.c_x + r.l, r.c_y + r.w * r.d_y, r.c_z + r.w * r.d_z);

		output.push_back(Segment_3(p_1, p_2));
		output.push_back(Segment_3(p_2, p_4));
		output.push_back(Segment_3(p_1, p_3));
		output.push_back(Segment_3(p_3, p_4));
	}

	return output;
}


void generate_gnuplot_script(vector<Segment_3> segs)
{

	int i = 0;
	int j = 0;
	string script = "";
	while (i < segs.size())
	{
		Segment_3 s1 = segs[i];
		Segment_3 s2 = segs[i + 1];
		Segment_3 s3 = segs[i + 2];
		Segment_3 s4 = segs[i + 3];

		Point_3 p1 = s1.source();
		Point_3 p2 = s2.source();
		Point_3 p3 = s2.end();
		Point_3 p4 = s4.source();

		string string_1 = to_string(CGAL::to_double(p1.x())) + ", " + to_string(CGAL::to_double(p1.y())) + ", " + to_string(CGAL::to_double(p1.z()));
		string string_2 = to_string(CGAL::to_double(p2.x())) + ", " + to_string(CGAL::to_double(p2.y())) + ", " + to_string(CGAL::to_double(p2.z()));
		string string_3 = to_string(CGAL::to_double(p3.x())) + ", " + to_string(CGAL::to_double(p3.y())) + ", " + to_string(CGAL::to_double(p3.z()));
		string string_4 = to_string(CGAL::to_double(p4.x())) + ", " + to_string(CGAL::to_double(p4.y())) + ", " + to_string(CGAL::to_double(p4.z()));

		string command = "set object " + to_string(++j) + " polygon from " + string_1 + " to " + string_2 + " to " + string_3 + " to " + string_4 + " to " + string_1 + " fillstyle solid lw 0\n";
		script += command;
		i += 4;
	}

	script += "unset border\n";
	script += "unset tics\n";
	script += "set xrange [0:1]\n";
	script += "set yrange [0:1]\n";
	script += "set zrange [0:1]\n";
	script += "set pointsize 0.001\n";
	script += "splot sqrt(-1)";

	ofstream file;
	file.open("plot_rectangle_script.txt");
	file << script;
	file.close();
}


void dynamic_seg_input_file(vector<Rectangle> rects)
{
	ofstream file;
	file.open("segments.txt");
	for (auto r : rects)
	{
		Point_2 p = Point_2(r.c_y, r.c_z);
		Point_2 q = Point_2(r.c_y + r.w * r.d_y, r.c_z + r.w * r.d_z);

		string line = to_string(r.c_y) + " " + to_string(r.c_z) + " " + to_string(r.c_y + r.w * r.d_y) + " " + to_string(r.c_z + r.w * r.d_z);
		line += " " + to_string(r.c_x) + " " + to_string(r.c_x + r.l) + "\n";

		file << line;
	}

	file.close();
}

int main()
{
	int number_of_rectangles;

	cin >> number_of_rectangles;
	vector<Rectangle> rects = generate_rectangles(number_of_rectangles);
	vector<Segment_3> segs = rect_to_seg(rects);
	generate_gnuplot_script(segs);
	dynamic_seg_input_file(rects);
}