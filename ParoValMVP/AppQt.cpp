#include "AppQt.hpp"
#include <QMessageBox>
#include "AStar.hpp"
#include "MVPParams.hpp"

void AppQt::start()
{
    if (!_initialized) {
        QMessageBox msgBox;
        msgBox.setText("Application was not configured properly.");
        msgBox.exec();
        return;
    }
    _running = true;
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
    _running = false;
}

void AppQt::close()
{
    if (_running)return;
    cv::destroyAllWindows();
}

void AppQt::readParameters(const MVPParams& params)
{
    _initialized = true;
}

void AppQt::_loadMap()
{
    _map = std::make_shared<Map>(_map_file_name);
    _map->load();
}

void AppQt::_showMap()
{
    if (_show_visualization == false)return;
    cv::imshow("A* Visualization", _map->getCanvas());
    cv::waitKey(1);
}

void AppQt::_createAStarPath()
{
    //auto start = _graph->getRandomNode();
    //auto goal = _graph->getRandomNode();
    auto start = _graph->getNodeFromIndex_1d(120);
    auto goal = _graph->getNodeFromIndex_1d(204);

    AStar astar(_graph, start, goal, _map->getCanvas(), _show_visualization);
    auto path = astar.searchPath();
    _path = std::make_shared<Path>(_map->getCanvas(), path, _graph, _smooth_rate_alpha, _smooth_rate_beta, _output_file_name);
}

void AppQt::_createGrid()
{
    if (_gridShouldBeShown == true) {
        _grid = std::make_shared<Grid>(_map->getWidth(), _map->getHeight(), _grid_cell_size);
    }
}

void AppQt::_showGrid()
{
    if (_show_visualization == false)return;
    if (_gridShouldBeShown == true) {
        _grid->drawGrid(_map->getCanvas());
    }
}

void AppQt::_createGraph()
{
    _graph = std::make_shared<Graph>(_map->getImage(), _grid_cell_size, _obstacle_cost_factor);
    _graph->createGraph();
}

void AppQt::_showGraph()
{
    if (_show_visualization == false)return;
    _graph->drawGraph(_map->getCanvas());
}

void AppQt::_showNodeCosts()
{
    if (_show_visualization == false)return;
    _graph->drawNodeCosts(_map->getCanvas());
}

void AppQt::_showAStarPath() {
    if (_show_visualization == false)return;
    _path->drawPath();
}

void AppQt::_createSmoothPath()
{
    _path->createSmoothPath();
}

void AppQt::_showSmoothPath()
{
    if (_show_visualization == false)return;
    _path->drawSmoothPath();
}

void AppQt::_exportSmoothPath()
{
    _path->writeToFile();
    std::cout << "Smoothed path was exported to the output file <" << _output_file_name << ">\n";
}