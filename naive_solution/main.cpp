#include "naive_algorithm.h";


struct Property 
{
	int integer;
};

struct TestStruct 
{
	Property* property;
};

vector<TestStruct> ints_to_structs(vector<int> integers)
{
	vector<TestStruct> structs = vector<TestStruct>();
	for (int i = 0; i < integers.size(); ++i) 
	{
		TestStruct s = TestStruct();
		structs.push_back(s);
		structs.back().property = new Property();
		structs.back().property->integer = integers[i];
	}

	return structs;
};

int main() 
{
	/*
	vector<int> integers = vector<int>();

	integers.push_back(1);
	integers.push_back(2);
	integers.push_back(3);

	vector<TestStruct> structs = ints_to_structs(integers);

	cout << structs[0].property->integer;*/
	
	vector<Segment> segments = read_segments_from_file();
	vector<Event> events = events_from_segments(segments);

	cout << "read segments\n";
	cout << events[0];

	sort(events.begin(), events.end());
	

	vector<Intersection> intersections = compute_intersections_naive(events);
	cout << "\n";
	cout << intersections.size();

}