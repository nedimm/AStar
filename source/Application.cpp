#include "../include/Application.hpp"
#include "../include/Map.hpp"
#include "../include/AStar.hpp"

void Application::start()
{
	_readParameters();
    _loadMap();
    //_createGrid();
    _createGraph();
    //_runAStar();
    _showMainWindow();
}

void Application::_readParameters() {
	_parameter_server.setParameterFile("parameters.csv");
	_parameter_server.updateParametersFromFile();
	_grid_cell_size = _parameter_server.getParameter("grid_cell_size");
}

void Application::_loadMap()
{
	cv::String file_name = _parameter_server.getParameterString("map_file_name");
	_map = std::make_shared<Map>(file_name);
	_map->load();
}

void Application::_showMainWindow()
{
    namedWindow("A* Visualization", cv::WINDOW_AUTOSIZE);
    //_showGrid();
    _showGraph();
    _showMap();
    _runAStar();
    cv::waitKey(0);
}

void Application::_showMap()
{
    cv::imshow("A* Visualization", _map->getCanvas());
    cv::waitKey(1);
}

void Application::_runAStar()
{
    auto start = _graph->getRandomNode();
    auto goal = _graph->getRandomNode();
    cv::circle(_map->getCanvas(), cv::Point(start.x_pos, start.y_pos), 5, cv::Scalar(0, 255, 0), 3);
    cv::circle(_map->getCanvas(), cv::Point(goal.x_pos, goal.y_pos), 5, cv::Scalar(0, 255, 0), 3);
    cv::imshow("A* Visualization", _map->getCanvas());
    cv::waitKey(1);
    
    AStar astar(_graph, start, goal, _map->getCanvas());
    astar.search();
}

void Application::_createGrid()
{
    _grid = std::make_shared<Grid>(_map->getWidth(), _map->getHeight(), _grid_cell_size);
}

void Application::_showGrid()
{
    _grid->drawGrid(_map->getCanvas());
}

void Application::_createGraph()
{
    _graph = std::make_shared<Graph>(_map->getImage(), _grid_cell_size);
    _graph->createGraph();
}

void Application::_showGraph()
{
    _graph->drawGraph(_map->getCanvas());
}
