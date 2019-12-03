#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

struct Node
{
	//bool operator == (const Node& o) { return x_index == o.x_index && y_index == o.y_index; }
	//bool operator < (const Node& o) { return distance + cost_so_far < o.distance + o.cost_so_far; }
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

class Graph
{
public:
    Graph(cv::Mat& map_image, int cell_size);
    void createGraph();
    void drawGraph(cv::Mat& image_to_draw_on);
    void _drawNodeText(cv::Mat& image_to_draw_on, std::vector<Node>::value_type node);
	int getMaxWidth();
	int getMaxHeight();
	int getCellSize();
    Node& getRandomNode() { return _nodes[0]; }
    Node& getNodeFromIndex_1d(int index_1d);

protected:
    bool _nodeIsDriveable(int i, int j);
    void _createNodes();
    void _addLeftNeighbor(const Node& node);
    void _addBottomNeighbor(const Node& node);
    void _createEdges();
    void _drawNodes(cv::Mat& image_to_draw_on);
    void _drawEdges(cv::Mat& image_to_draw_on);

    std::vector<Node> _nodes;
    std::vector<std::vector<int>> _node_index_matrix;
    cv::Mat& _map_image;
    int _cell_size;
    int _height;
    int _width;
	
	// drawing params
    cv::Scalar _node_color = cv::Scalar(255, 0, 0);
    cv::Scalar _edge_color = cv::Scalar(255, 0, 0);
    int _node_radius = 2;
    int _thickness = 1;
    bool _draw_node_text = false;
};
