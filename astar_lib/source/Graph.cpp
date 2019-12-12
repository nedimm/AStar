#include "../include/Graph.hpp"

Graph::Graph(cv::Mat& map_image, int cell_size, float obstacle_cost_factor):
_map_image(map_image)
, _cell_size(cell_size)
, _width(map_image.cols)
, _height(map_image.rows)
, _obstacle_cost_factor(obstacle_cost_factor)
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

Node* Graph::getRandomNode()
{
    int r = std::rand() % _nodes.size();
    return &_nodes[r];
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

    for (int i = 0; i < _nodes.size(); ++i)
    {
		_setNodeMovementCosts(i);
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
    }
}

void Graph::_setNodeMovementCosts(int index_1d)
{
    Node* node = &_nodes[index_1d];
    // set left obstacle distance
    if (!_hasLeftNeighbor(_nodes[index_1d]))
    {
        float distance = 0.f;
        for (int i = 1; i < _cell_size; ++i)
        {
            distance += 1.f;
            if (!_nodeIsDriveable(node->y_pos, node->x_pos - i))
            {
                node->movement_cost = _base_movement_cost + _obstacle_cost_factor * ((_cell_size-distance)/_cell_size);
                break;
            }
        }
    }
    // set right obstacle distance
    if (!_hasRightNeighbor(_nodes[index_1d]))
    {
        float distance = 0.f;
        for (int i = 1; i < _cell_size; ++i)
        {
            distance += 1.f;
            if (!_nodeIsDriveable(node->y_pos, node->x_pos + i))
            {
                node->movement_cost = std::max(node->movement_cost, _base_movement_cost + _obstacle_cost_factor * ((_cell_size - distance) / _cell_size));
                break;
            }
        }
    }
    // set top obstacle distance
    if (!_hasTopNeighbor(_nodes[index_1d]))
    {
        float distance = 0.f;
        for (int i = 1; i < _cell_size; ++i)
        {
            distance += 1.f;
            if (!_nodeIsDriveable(node->y_pos - i, node->x_pos))
            {
                node->movement_cost = std::max(node->movement_cost, _base_movement_cost + _obstacle_cost_factor * ((_cell_size - distance) / _cell_size));
                break;
            }
        }
    }
    // set bottom obstacle distance
    if (!_hasBottomNeighbor(_nodes[index_1d]))
    {
        float distance = 0.f;
        for (int i = 1; i < _cell_size; ++i)
        {
            distance += 1.f;
            if (!_nodeIsDriveable(node->y_pos + i, node->x_pos))
            {
                node->movement_cost = std::max(node->movement_cost, _base_movement_cost + _obstacle_cost_factor * ((_cell_size - distance) / _cell_size));
                break;
            }
        }
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

bool Graph::_hasLeftNeighbor(const Node& node)
{
    if (node.x_index == 0)return false;
    for (auto neighbor_1D : node.neighbors_1d)
    {
        auto x_index = _nodes[neighbor_1D].x_index;
        auto y_index = _nodes[neighbor_1D].y_index;
        if (y_index == node.y_index && x_index == node.x_index - 1) return true;
    }
    return false;
}

bool Graph::_hasRightNeighbor(const Node& node)
{
    if (node.x_index == _width - 1)return false;
    for (auto neighbor_1D : node.neighbors_1d)
    {
        auto x_index = _nodes[neighbor_1D].x_index;
        auto y_index = _nodes[neighbor_1D].y_index;
        if (y_index == node.y_index && x_index == node.x_index + 1) return true;
    }
    return false;
}

bool Graph::_hasTopNeighbor(const Node& node)
{
    if (node.y_index == 0)return false;
    for (auto neighbor_1D : node.neighbors_1d)
    {
        auto x_index = _nodes[neighbor_1D].x_index;
        auto y_index = _nodes[neighbor_1D].y_index;
        if (y_index == node.y_index - 1 && x_index == node.x_index) return true;
    }
    return false;
}

bool Graph::_hasBottomNeighbor(const Node& node)
{
    if (node.y_index == _height - 1)return false;
    for (auto neighbor_1D : node.neighbors_1d)
    {
        auto x_index = _nodes[neighbor_1D].x_index;
        auto y_index = _nodes[neighbor_1D].y_index;
        if (y_index == node.y_index + 1 && x_index == node.x_index) return true;
    }
    return false;
}

void Graph::drawGraph(cv::Mat& canvas)
{
    if (_graph_should_be_shown == true) {
        _drawNodes(canvas);
        _drawEdges(canvas);
    }
}

void Graph::_drawNodeText(cv::Mat& canvas, std::vector<Node>::value_type node)
{
    if (!_draw_node_text)return;
    //cv::String text = cv::String( "(" +std::to_string(node.x_index) + "," + std::to_string(node.y_index) + "," + std::to_string(node.index_1d) + ")");
    cv::String text = cv::String(std::to_string(node.index_1d));

    putText(canvas, text, cv::Point(node.x_pos+1, node.y_pos +1),
            cv::FONT_HERSHEY_PLAIN, 0.8, cv::Scalar(0, 0, 0), 1);
}

cv::Scalar Graph::_getNodeColor(Node& node) {
	double blue = 0.0;
	double penalty = node.movement_cost - base_cost;
	double green = 255 - (255 * penalty);
	double red = 0 + (255 * penalty);
	return cv::Scalar(blue, green, red);
}

void Graph::drawNodeCosts(cv::Mat& canvas) {
	for (auto node : _nodes)
	{
		cv::circle(canvas, cv::Point(node.x_pos, node.y_pos), _node_radius+3, _getNodeColor(node), _thickness+2);
		_drawNodeText(canvas, node);
	}
}


void Graph::_drawNodes(cv::Mat& canvas)
{
    for (auto node : _nodes)
    {
        cv::circle(canvas, cv::Point(node.x_pos, node.y_pos), _node_radius, _default_node_color, _thickness);
        _drawNodeText(canvas, node);
    }
}

void Graph::_drawEdges(cv::Mat& canvas)
{
    for (auto node : _nodes)
    {
        for (auto neighbor_1D : node.neighbors_1d)
        {
            Node& neighbor = _nodes[neighbor_1D];
            cv::line(canvas, cv::Point(node.x_pos, node.y_pos), cv::Point(neighbor.x_pos, neighbor.y_pos), _edge_color, _thickness);
        }
    }
}

Node* Graph::getNodeFromIndex_1d(int index_1d)
{
    return &_nodes[index_1d];
}

bool Graph::_nodeIsDriveable(int row, int column)
{
    return _map_image.at<uchar>(row, column);
}
