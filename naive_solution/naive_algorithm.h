#include "input_reader.h";

void process_event(vector<Event>& active_events, Event& new_event);
vector<Intersection> compute_intersections_naive(vector<Event> events);