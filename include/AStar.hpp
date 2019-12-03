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
	AStar(std::shared_ptr<Graph> graph, Node& start, Node& end);
    void search();
	float calculateEstimatedDistance(Node& from_node, Node& to_node);

protected:
    std::shared_ptr<Graph> _graph;
    Node& _start;
    Node& _goal;
    PriorityQueue<int, double> frontier;
	float _movement_cost = 1;
};