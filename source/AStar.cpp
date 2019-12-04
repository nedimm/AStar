#include "../include/AStar.hpp"

AStar::AStar(std::shared_ptr<Graph> graph, Node& start, Node& end, cv::Mat& canvas) :
    _graph(graph),
    _start(start),
    _goal(end),
    _canvas(canvas)
{
	_drawNode(_start.index_1d, _start_goal_node_radius, _start_node_color, _start_goal_node_thickness);
	_drawNode(_goal.index_1d, _start_goal_node_radius, _goal_node_color, _start_goal_node_thickness);
}

void AStar::_drawNode(const int current, int radius, cv::Scalar color, int thickness)
{
    auto node = _graph->getNodeFromIndex_1d(current);
    cv::circle(_canvas, cv::Point(node.x_pos, node.y_pos), radius, color, thickness);
    cv::imshow("A* Visualization", _canvas);
    cv::waitKey(1);
}

float AStar::heuristic(Node& from_node) const
{
    const int dx = abs(from_node.x_index - _goal.x_index);
    const int dy = abs(from_node.y_index - _goal.y_index);
    return _movement_cost * (dx + dy);
}


std::vector<int> AStar::searchPath()
{
    frontier.put(_start.index_1d, 0);
    _start.came_from_1d = _start.index_1d;
    _start.cost_so_far = 0;

    while (!frontier.empty()) {
        const int current = frontier.get();
        _visited_1d.insert(current);
        _drawNode(current, _exploration_node_radius, _exploration_node_color, _exploration_node_thickness);
        if (current == _goal.index_1d) {
            break;
        }
        auto current_node = _graph->getNodeFromIndex_1d(current);
        for (auto node_1d : current_node.neighbors_1d) {
            if (_visited_1d.count(node_1d) > 0)continue;
            auto next_node = _graph->getNodeFromIndex_1d(node_1d);
            _drawNode(node_1d, _exploration_node_radius, _exploration_node_color, _exploration_node_thickness);

            const float new_cost = current_node.cost_so_far + _movement_cost;
            if (next_node.cost_so_far == _goal.cost_so_far
                || new_cost < next_node.cost_so_far) {
                next_node.cost_so_far = new_cost;
                const double priority = new_cost + heuristic(next_node);
                frontier.put(next_node.index_1d, priority);
                next_node.came_from_1d = current;
            }
        }
		
    }
	return _backpropagate();
}

std::vector<int> AStar::_backpropagate()
{
	std::vector<int> path;
	int current = _goal.index_1d;
	while (current != _start.index_1d) {
		path.push_back(current);
		auto current_node = _graph->getNodeFromIndex_1d(current);
		current = current_node.came_from_1d;
	}
	return path;
}
