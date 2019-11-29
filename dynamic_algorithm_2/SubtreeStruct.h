#pragma once
#include "Node.h"

enum NextPrioritySubtreeType { RAB, LAB, AB, RLAB, LEAF };


struct Subtrees
{
	NextPrioritySubtreeType type;

	Node* t_left = nullptr;
	Node* t_right = nullptr;
	Node* t_above = nullptr;
	Node* t_below = nullptr;
};