#include "../include/Graph.hpp"

Graph::Graph(cv::Mat& map_image, int cell_size):
_map_image(map_image)
, _cell_size(cell_size)
, _width(map_image.cols)
, _height(map_image.rows)
{
    std::srand(std::time(0));
}

int Graph::getMaxWidth() {
	return _width;
}

int Graph::getMaxHeight() {
	return _height;
}

int Graph::getCellSize() {
	return _cell_size;
}

Node& Graph::getRandomNode()
{
    int r = std::rand() % _nodes.size() + 1;
    auto it = _nodes.begin();
    std::advance(it, r);
    return *it;
}

void Graph::createGraph()
{
    if (_map_image.empty())
    {
        std::cout << "Image empty" << std::endl;
        return;
    }

   _createNodes();
   _createEdges();
}

void Graph::_createNodes()
{
    int cnt = 0;
    for (int i = 0; i < _height; i += _cell_size)
    {
        std::vector<int> node_index_row;
        for (int j = 0; j < _width; j += _cell_size)
        {
            if (_nodeIsDriveable(i, j))
            {
                node_index_row.push_back(cnt);
                _nodes.push_back({ static_cast<float>(j),static_cast<float>(i),j/_cell_size,i/_cell_size, cnt++ });
            }else
            {
                node_index_row.push_back(-1);
            }
        }
        _node_index_matrix.push_back(node_index_row);

    }
}

void Graph::_addLeftNeighbor(const Node& node)
{
    if (node.x_index > 0)
    {
        int left_coordinate = node.x_pos - 1;
        bool could_be_driveable = true;
        while(left_coordinate >= node.x_pos - _cell_size && could_be_driveable)
        {
            could_be_driveable = _nodeIsDriveable(node.y_pos, left_coordinate--);
        }
        if (could_be_driveable)
        {
            int index_left = _node_index_matrix[node.y_index][node.x_index - 1];
            _nodes[node.index_1d].neighbors_1d.push_back(index_left);
            _nodes[index_left].neighbors_1d.push_back(node.index_1d);
        }
        /*if (_nodeIsDriveable(node.y_pos, node.x_pos - _cell_size))
        {
            int index_left = _node_index_matrix[node.y_index][node.x_index-1];
            _nodes[node.index_1d].neighbors_1d.push_back(index_left);
        }*/
    }
}

void Graph::_addBottomNeighbor(const Node& node)
{
    if(node.y_index - 1 < _height / _cell_size)
    {
        int bottom_coordinate = node.y_pos + 1;
        bool could_be_driveable = true;
        while (bottom_coordinate <= node.y_pos + _cell_size && could_be_driveable)
        {
            could_be_driveable = _nodeIsDriveable(bottom_coordinate++, node.x_pos);
        }
        if (could_be_driveable)
        {
            int index_bottom = _node_index_matrix[node.y_index + 1][node.x_index];
            _nodes[node.index_1d].neighbors_1d.push_back(index_bottom);
            _nodes[index_bottom].neighbors_1d.push_back(node.index_1d);
        }
        /*if (_nodeIsDriveable(node.y_pos + _cell_size, node.x_pos))
        {
            int index_bottom = _node_index_matrix[node.y_index+1][node.x_index];
            _nodes[node.index_1d].neighbors_1d.push_back(index_bottom);
        }*/
    }
}

void Graph::_createEdges()
{
    for (auto node : _nodes)
    {
        _addLeftNeighbor(node);
        _addBottomNeighbor(node);
    }
}



void Graph::drawGraph(cv::Mat& image_to_draw_on)
{
    _drawNodes(image_to_draw_on);
    _drawEdges(image_to_draw_on);
}

void Graph::_drawNodeText(cv::Mat& image_to_draw_on, std::vector<Node>::value_type node)
{
    if (!_draw_node_text)return;
    //cv::String text = cv::String( "(" +std::to_string(node.x_index) + "," + std::to_string(node.y_index) + "," + std::to_string(node.index_1d) + ")");
    cv::String text = cv::String(std::to_string(node.index_1d));

    putText(image_to_draw_on, text, cv::Point(node.x_pos+1, node.y_pos +1),
            cv::FONT_HERSHEY_PLAIN, 0.8, cv::Scalar(0, 0, 0), 1);
}

void Graph::_drawNodes(cv::Mat& image_to_draw_on)
{
    for (auto node : _nodes)
    {
        cv::circle(image_to_draw_on, cv::Point(node.x_pos, node.y_pos), _node_radius, _node_color, _thickness);
        _drawNodeText(image_to_draw_on, node);
    }
}

void Graph::_drawEdges(cv::Mat& image_to_draw_on)
{
    for (auto node : _nodes)
    {
        for (auto neighbor_1D : node.neighbors_1d)
        {
            Node& neighbor = _nodes[neighbor_1D];
            cv::line(image_to_draw_on, cv::Point(node.x_pos, node.y_pos), cv::Point(neighbor.x_pos, neighbor.y_pos), _edge_color, _thickness);
        }
    }
}

Node& Graph::getNodeFromIndex_1d(int index_1d)
{
    return _nodes[index_1d];
}

bool Graph::_nodeIsDriveable(int row, int column)
{
    return _map_image.at<uchar>(row, column);
}
