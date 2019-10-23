#include "input_reader.h";

vector<Segment> read_segments_from_file() 
{
	ifstream file;
	file.open("segments.txt");
	vector<Segment> segments = vector<Segment>();

	string line;
	int number_of_segments;
	getline(file, line);
	number_of_segments = stoi(line);

	for (int i = 0; i < number_of_segments; ++i) 
	{
		getline(file, line);
		istringstream iss(line);
		vector<string> data(istream_iterator<string>{iss}, istream_iterator<string>());

		double x1 = stod(data[0]);
		double y1 = stod(data[1]);
		double x2 = stod(data[2]);
		double y2 = stod(data[3]);
		double birth_time = stod(data[4]);
		double death_time = stod(data[5]);

		segments.push_back(Segment());

		segments.back().seg = new Segment_2(
			Point_2(x1, y1),
			Point_2(x2, y2)
		);

		segments.back().birth = birth_time;
		segments.back().death = death_time;

		segments.back().id = i;
	}

	return segments;
}

vector<Event> events_from_segments(vector<Segment>& segments) 
{
	vector<Event> events = vector<Event>();

	for (int i = 0; i < segments.size(); ++i) 
	{
		cout << *(segments[i].seg);

		//Set birth event
		events.push_back(Event());
		events.back().segment = &segments[i];
		events.back().is_birth_event = true;
		

		//Set death event
		events.push_back(Event());
		events.back().segment = &segments[i];
		events.back().is_birth_event = false;
	}

	return events;

}
