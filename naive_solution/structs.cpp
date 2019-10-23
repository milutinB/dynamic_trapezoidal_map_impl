#include "structs.h";

bool operator<(Event e1, Event e2) 
{
	return e1.time() < e2.time();
}

ostream& operator<<(ostream& s, Event& e) 
{
	if (e.is_birth_event) 
	{
		s<< "BIRTH EVENT \n";
	}
	else 
	{
		s << "DEATH EVENT \n";
	}

	s << *(e.segment->seg);
	s << "\n";

	return s;
}

bool operator==(const Event& e1, const Event& e2) 
{
	return e1.segment->id == e2.segment->id;
}

CGAL::Object intersect(Segment& s1, Segment& s2) 
{
	Segment_2 cgal_seg1 = *(s1.seg);
	Segment_2 cgal_seg2 = *(s2.seg);

	return CGAL::intersection(cgal_seg1, cgal_seg2);
}