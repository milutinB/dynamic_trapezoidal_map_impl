#pragma once

#include "Segment.h";


ostream& operator<<(ostream& c, Segment& segment)
{
	c << segment.seg;
	c << "\n";

	return c;
}