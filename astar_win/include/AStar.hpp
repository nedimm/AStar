#pragma once
#include "Graph.hpp"
#include <unordered_map>

template<typename T, typename priority_t>
struct PriorityQueue {
    typedef std::pair<priority_t, T> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>,
        std::greater<PQElement>> elements;

    inline bool empty() const {
        return elements.empty();
    }

    inline void put(T item, priority_t priority) {
        elements.emplace(priority, item);
    }

    T get() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};

class AStar {
public:
	AStar(std::shared_ptr<Graph> graph, Node* start, Node* end, cv::Mat& canvas);
	std::vector<int> searchPath();
	
	float heuristic(Node* from_node) const;

protected:
    void _drawNode(const int current, int radius, cv::Scalar color, int thickness);
    void _drawStartNode();
    void _drawExplorationNode(const int current);



	std::vector<int> _backpropagate();
	std::shared_ptr<Graph> _graph;
    Node* _start;
    Node* _goal;
    PriorityQueue<int, double> frontier;
	float _movement_cost = 1;
    cv::Mat& _canvas;
    std::set<int> _visited_1d;
	cv::Scalar _exploration_node_color = cv::Scalar(0, 165, 255);
	cv::Scalar _start_node_color = cv::Scalar(0, 255, 0);
	cv::Scalar _goal_node_color = cv::Scalar(0, 0, 255);

	int _start_goal_node_radius = 5;
	int _start_goal_node_thickness = 3;
	int _exploration_node_radius = 3;
	int _exploration_node_thickness = 2;

    bool _should_draw_start_node = true;
    bool _should_draw_exploration_node = true;




};