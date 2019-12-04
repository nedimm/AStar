#pragma once
#include <vector>

struct Node
{
	float x_pos;
	float y_pos;
	int x_index;
	int y_index;
	int index_1d;
	std::vector<int> neighbors_1d;
	int parent_index_1d;
	int came_from_1d;
	float estimated_distance; //h(n)
	float cost_so_far; //g(n)
};