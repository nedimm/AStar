#include "../include/AStar.hpp"

AStar::AStar(std::shared_ptr<Graph> graph, Node& start, Node& end):
	_graph(graph),
	_start(start),
	_goal(end)
{
}

void AStar::search()
{
    frontier.put(_start.index_1d, 0);
    came_from[_start.index_1d] = _start.index_1d;
    cost_so_far[_start.index_1d] = 0;

    while (!frontier.empty()) {
        int current = frontier.get();

        if (current == _goal.index_1d) {
            break;
        }
        auto current_node = _graph->getNodeFromIndex_1d(current);
        for (auto node : current_node.neighbors_1d) {

        }
    }
}
