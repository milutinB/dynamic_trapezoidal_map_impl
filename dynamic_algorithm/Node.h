#pragma once

#include "Cut.h"
#include "BoundingBox.h"

struct Node 
{
	Node* negative_child = nullptr;
	Node* positive_child = nullptr;
	Node* parent = nullptr;
	Cut cut;

	bool is_leaf();
};