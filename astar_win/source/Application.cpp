#include "../include/Application.hpp"
#include "Map.hpp"
#include "../include/AStar.hpp"

void Application::start()
{
    if (!_initialized) {
        std::cout << "Application was not configured properly, check the settings file <" << _params_file_name <<"> or command line arguments.\n";
        return;
    }
    _loadMap();
    _createGrid();
    _showGrid();
    _createGraph();
    _showGraph();
    _showMap();
    _showNodeCosts();
    _createAStarPath();
    _showAStarPath();
    _createSmoothPath();
    _showSmoothPath();
    _exportSmoothPath();
    cv::waitKey(0);
}

void Application::readParametersFromCSV() {
    _parameter_server.setParameterFile(_params_file_name);
    _parameter_server.updateParametersFromFile();
    _map_file_name = _parameter_server.getParameterString("map_file_name");
    _show_visualization = _parameter_server.getParameter("show_visualization");
    _grid_cell_size = _parameter_server.getParameter("grid_cell_size");
    _obstacle_cost_factor = _parameter_server.getParameter("obstacle_cost_factor");
    _smooth_rate_alpha = _parameter_server.getParameter("smooth_rate_alpha");
    _smooth_rate_beta = _parameter_server.getParameter("smooth_rate_beta");
    _output_file_name = _parameter_server.getParameterString("output_file_name");

    _initialized = true;
}

void Application::readParametersFromCommandLine(char** argv)
{
    std::istringstream mfn(argv[1]);
    if (!(mfn >> _map_file_name))return;

    std::istringstream vis(argv[2]);
    if (!(vis >> _show_visualization))return;

    std::istringstream gcs(argv[3]);
    if (!(gcs >> _grid_cell_size))return;

    std::istringstream ocf(argv[4]);
    if (!(ocf >> _obstacle_cost_factor))return;

    std::istringstream sra(argv[5]);
    if (!(sra >> _smooth_rate_alpha))return;

    std::istringstream srb(argv[6]);
    if (!(srb >> _smooth_rate_beta))return;

    std::istringstream fn(argv[7]);
    if (!(fn >> _output_file_name))return;
    
    _initialized = true;
}

void Application::_loadMap()
{
    _map = std::make_shared<Map>(_map_file_name);
    _map->load();
}

void Application::_showMap()
{
    if (_show_visualization == false)return;
    cv::imshow("A* Visualization", _map->getCanvas());
    cv::waitKey(1);
}

void Application::_createAStarPath()
{
    //auto start = _graph->getRandomNode();
    //auto goal = _graph->getRandomNode();
    auto start = _graph->getNodeFromIndex_1d(120);
    auto goal = _graph->getNodeFromIndex_1d(204);

    AStar astar(_graph, start, goal, _map->getCanvas(), _show_visualization);
    auto path = astar.searchPath();
    _path = std::make_shared<Path>(_map->getCanvas(), path, _graph, _smooth_rate_alpha, _smooth_rate_beta, _output_file_name);
}

void Application::_createGrid()
{
    if (_gridShouldBeShown == true) {
        _grid = std::make_shared<Grid>(_map->getWidth(), _map->getHeight(), _grid_cell_size);
    }
}

void Application::_showGrid()
{
    if (_show_visualization == false)return;
    if (_gridShouldBeShown == true) {
        _grid->drawGrid(_map->getCanvas());
    }
}

void Application::_createGraph()
{
    _graph = std::make_shared<Graph>(_map->getImage(), _grid_cell_size, _obstacle_cost_factor);
    _graph->createGraph();
}

void Application::_showGraph()
{
    if (_show_visualization == false)return;
    _graph->drawGraph(_map->getCanvas());
}

void Application::_showNodeCosts()
{
    if (_show_visualization == false)return;
    _graph->drawNodeCosts(_map->getCanvas());
}

void Application::_showAStarPath() {
    if (_show_visualization == false)return;
    _path->drawPath();
}

void Application::_createSmoothPath()
{
    _path->createSmoothPath();
}

void Application::_showSmoothPath()
{
    if (_show_visualization == false)return;
    _path->drawSmoothPath();
}

void Application::_exportSmoothPath()
{
    _path->writeToFile();
    std::cout << "Smoothed path was exported to the output file <" << _output_file_name << ">\n";
}
