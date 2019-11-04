#pragma once

#include "Node.h"


bool Node::is_leaf() 
{

	bool is_leaf = this->negative_child == nullptr && this->positive_child == nullptr;
	return is_leaf;
}