#include <algorithm>
#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h> 
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/intersections.h>
#include <ostream>
#include <fstream>


typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Point_3<Kernel> Point_3;
typedef CGAL::Segment_3<Kernel> Segment_3;
using namespace std;


struct Segment
{
	double birth, death;
	Segment_2* seg;
	int id;
};

struct Event
{
	Segment* segment;
	bool is_birth_event;
	double time() 
	{
		if (is_birth_event) 
		{
			return segment->birth;
		}
		else 
		{
			return segment->death;
		}
	}
};

struct Intersection 
{
	Segment* seg1;
	Segment* seg2;
	Point_2 intersection_point;
};


bool operator<(Event e1, Event e2);

ostream& operator<<(ostream& s, Event& e);

bool operator==(const Event& e1, const Event& e2);

CGAL::Object intersect(Segment& s1, Segment& s2);