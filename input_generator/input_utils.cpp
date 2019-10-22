#include "input_utils.h"


// creates a vector of random rectangle structs
vector<Rectangle> generate_rectangles(int number_of_rectangles)
{
	vector<Rectangle> output = vector<Rectangle>();
	srand(137);

	for (int i = 0; i < number_of_rectangles; ++i)
	{
		Rectangle rect = Rectangle();
		rect.c_x = ((double)rand() / RAND_MAX);
		rect.c_y = ((double)rand() / RAND_MAX);
		rect.c_z = ((double)rand() / RAND_MAX);
		rect.w = ((double)rand() / RAND_MAX);
		rect.l = ((double)rand() / RAND_MAX);
		rect.d_y = ((double)rand() / RAND_MAX);
		rect.d_z = ((double)rand() / RAND_MAX);

		double norm = sqrt(rect.d_y * rect.d_y + rect.d_z + rect.d_z);
		rect.d_y /= norm;
		rect.d_z /= norm;

		output.push_back(
			rect
		);
	}

	return output;
}
