#include "naive_algorithm.h";

void process_event(vector<Event>& active_events, Event& new_event) 
{
	cout << new_event.segment->id;
	cout << "\n";
	if (new_event.is_birth_event) 
	{
		active_events.push_back(new_event);
		cout << active_events.size();
		cout << "\n";
	}
	else 
	{
		cout << "hello from death";
		auto e = find(active_events.begin(), active_events.end(), new_event);

		if (e != active_events.end()) 
		{
			active_events.erase(e);
		}
	}
}

vector<Intersection> compute_intersections_naive(vector<Event> events) 
{
	vector<Intersection> intersections = vector<Intersection>();
	vector<Event> active_events = vector<Event>();

	for (int i = 0; i < events.size(); ++i) 
	{
		Event next_event = events[i];

		if (next_event.is_birth_event) 
		{
			for (int j = 0; j < active_events.size(); ++j) 
			{
				Event active_event = active_events[j];
				Point_2 intersection_point;

				CGAL::Object result = intersect(*next_event.segment, *active_event.segment);

				if (CGAL::assign(intersection_point, result)) 
				{
					intersections.push_back(Intersection());

					intersections.back().seg1 = next_event.segment;
					intersections.back().seg2 = next_event.segment;
					intersections.back().intersection_point = intersection_point;
				}

			}
		}

		process_event(active_events, next_event);
	}

	return intersections;
}