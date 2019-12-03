#include "../include/AStar.hpp"

AStar::AStar(std::shared_ptr<Graph> graph, Node& start, Node& end):
	_graph(graph),
	_start(start),
	_goal(end)
{
}

float AStar::calculateEstimatedDistance(Node& from_node, Node& to_node) {
	int dx = abs(from_node.x_index - to_node.x_index);
	int dy = abs(from_node.y_index - to_node.y_index);
	return _movement_cost * (dx + dy);
}


void AStar::search()
{
    frontier.put(_start.index_1d, 0);
    _start.came_from_1d = _start.index_1d;
	_start.cost_so_far = 0;

    while (!frontier.empty()) {
        int current = frontier.get();

        if (current == _goal.index_1d) {
            break;
        }
        auto current_node = _graph->getNodeFromIndex_1d(current);
        for (auto node_1d : current_node.neighbors_1d) {
			auto next_node = _graph->getNodeFromIndex_1d(node_1d);
			float new_cost = current_node.cost_so_far + _movement_cost;
        }
    }
}
