#pragma once

#include "Node.h"


bool Node::is_leaf()
{
	bool is_leaf = this->negative_child == nullptr && this->positive_child == nullptr;
	return is_leaf;
}


bool Node::compare_nodes(Node* node)
{
	//cout << "compare nodes\n";
	if (is_leaf() && node->is_leaf())
	{
		//cout << "two leaves\n";
		return true;
	}

	//cout << "not two leaves\n";

	if ((!is_leaf() && node->is_leaf()) || (is_leaf() && !node->is_leaf()))
	{
		cout << "leaf and non leaf\n";

		if (!is_leaf()) 
		{
			cout << "\n dynamic \n";
			cout << *cut;
		}
		else 
		{
			cout << "\n naive \n";
			cout << *node->cut;

			cout << intersect_segments(node->cut->segment->seg, node->cut->intersecting_segment->seg);

		}

		return false;
	}

	//cout << "not leaf and non leaf\n";

	if (cut->compare_cuts(node->cut))
	{
	//	cout << *cut << "\n";
		return negative_child->compare_nodes(node->negative_child) && positive_child->compare_nodes(node->positive_child);
	}
	else 
	{
		//std::cout << "NOPE\n" << *node->cut << "\n";
		//std::cout << *cut << "\n";
	}


	return false;

}

double Node::priority()
{

	if (is_leaf())
	{
		return 1000000000;
	}

	return cut->priority();
}
