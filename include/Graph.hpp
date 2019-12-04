#pragma once
#include "Node.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

class Graph
{
public:
    Graph(cv::Mat& canvas, int cell_size);
    void createGraph();
    void drawGraph(cv::Mat& image_to_draw_on);
	int getMaxWidth();
	int getMaxHeight();
	int getCellSize();
    Node* getRandomNode();
    Node* getNodeFromIndex_1d(int index_1d);

protected:
    bool _nodeIsDriveable(int i, int j);
    void _createNodes();
    void _addLeftNeighbor(const Node& node);
    void _addBottomNeighbor(const Node& node);
    void _createEdges();
    void _drawNodes(cv::Mat& image_to_draw_on);
    void _drawNodeText(cv::Mat& image_to_draw_on, std::vector<Node>::value_type node);
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
    bool _graph_should_be_shown = true;
};
