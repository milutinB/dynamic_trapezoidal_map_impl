#pragma once

#include "Cut.h"
#include "BoundingBox.h"

struct Node
{
	Node* negative_child = nullptr;
	Node* positive_child = nullptr;
	Node* parent = nullptr;
	Cut* cut;
	double priority();

	bool is_leaf();

	bool compare_nodes(Node* node); 
};