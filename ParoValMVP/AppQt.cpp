#include "AppQt.hpp"
#include <QMessageBox>
#include "AStar.hpp"

void AppQt::setParameters(const MVPParams& params)
{
    if (_pathFindingIsRunning)return;
    _params = params;
    _initialized = true;
    if (_graph != nullptr)
        _graph->setDrawNodeTextIsShown(_params.showNodeText);
}

void AppQt::_showOptimizationRun()
{
    if (!_initialized) {
        QMessageBox msgBox;
        msgBox.setText("Application was not configured properly.");
        msgBox.exec();
        return;
    }
    _pathFindingIsRunning = true;

    _loadMap();
    _createGrid();
    _showGrid();
    _createGraph();
    _showGraph();
    _showMap();
    _createAStarPaths();
    _createSmoothPaths();
    _showSmoothPaths();
    _pathFindingIsRunning = false;
}

void AppQt::visualizeOptimizationRun()
{
    _paroval_params = _optimization_run_reader.readOptimizationRun();
    _showOptimizationRun();
}

MVPParams AppQt::getParameters()
{
    return _params;
}

void AppQt::displayEnvironment()
{
    if (_pathFindingIsRunning)return;
    _createCanvas();
    _showCanvas();
}

int AppQt::getLastNodeIndex()
{
    if (nullptr == _graph)return 0;
    return _graph->getNumberOfNodes() - 1;
}

void AppQt::start()
{
    if (!_initialized) {
        QMessageBox msgBox;
        msgBox.setText("Application was not configured properly.");
        msgBox.exec();
        return;
    }
    _pathFindingIsRunning = true;
    
    _loadMap();
    _createGrid();
    _showGrid();
    _createGraph();
    _showGraph();
    _showMap();
    _createAStarPath();
    _createSmoothPath();
    _showSmoothPath();
    _exportSmoothPath();
    _pathFindingIsRunning = false;
}

void AppQt::close() const
{
    if (_pathFindingIsRunning)return;
    cv::destroyAllWindows();
}

void AppQt::_createCanvas()
{
    _loadMap();
    _createGrid();
    _createGraph();
}

void AppQt::_showCanvas() const
{
    _showGrid();
    _showGraph();
    _showMap();
}

void AppQt::_loadMap()
{
    _map = std::make_shared<Map>(_params.mapFileName);
    _map->load();
}

void AppQt::_showMap() const
{
    cv::imshow("ParOVal Path Visualization", _map->getCanvas());
    cv::waitKey(1);
}

void AppQt::_createAStarPath()
{
    Node* start;
    Node* end;
    if (_params.randomStartEnd)
    {
        start = _graph->getRandomNode();
        end = _graph->getRandomNode();
    }else
    {
        start = _graph->getNodeFromIndex_1d(_params.startPoint);
        end = _graph->getNodeFromIndex_1d(_params.endPoint);
    }
   
    AStar astar(_graph, start, end, _map->getCanvas(), true);
    auto path = astar.searchPath();
    _path = std::make_shared<Path>(_map->getCanvas(), path, _graph, _params.smoothAlpha, _params.smoothBeta, _output_file_name);
}

void AppQt::_createAStarPaths()
{
    _paths.clear();
    for (auto pp : _paroval_params)
    {
        std::shared_ptr<Graph> graph = std::make_shared<Graph>(_map->getImage(), _params.gridCellSize, pp.obstacle_cost_factor);
        graph->createGraph();
        Node* start;
        Node* end;
        if (_params.randomStartEnd)
        {
            start = graph->getNodeFromIndex_1d(0);
            end = graph->getNodeFromIndex_1d(graph->getNumberOfNodes()-1);
        }
        else
        {
            start = graph->getNodeFromIndex_1d(_params.startPoint);
            end = graph->getNodeFromIndex_1d(_params.endPoint);
        }

        AStar astar(graph, start, end, _map->getCanvas(), false);
        auto path = astar.searchPath();
        _paths.push_back(std::make_shared<Path>(_map->getCanvas(), path, graph, pp.smooth_rate_alpha, pp.smooth_rate_beta, ""));
    }
}

void AppQt::_createGrid()
{
    _grid = std::make_shared<Grid>(_map->getWidth(), _map->getHeight(), _params.gridCellSize);
}

void AppQt::_showGrid() const
{
    if (nullptr == _grid || !_params.showGrid)return;
    _grid->drawGrid(_map->getCanvas());
}

void AppQt::_createGraph()
{
    _graph = std::make_shared<Graph>(_map->getImage(), _params.gridCellSize, _params.obstacleCostFactor);
    _graph->setDrawNodeTextIsShown(_params.showNodeText);
    _graph->createGraph();
}

void AppQt::_showGraph() const
{
    if (nullptr == _graph || !_params.showGraph)return;
    _graph->drawGraph(_map->getCanvas());
    if (_params.showNodeCosts)
        _graph->drawNodeCosts(_map->getCanvas());
}

void AppQt::_showAStarPath() const
{
    _path->drawPath();
}

void AppQt::_createSmoothPath()
{
    _path->createSmoothPath();
}

void AppQt::_createSmoothPaths()
{
    for (auto path : _paths)
    {
        path->createSmoothPath();
    }
}

void AppQt::_showSmoothPath()
{
    _path->drawSmoothPath();
}

void AppQt::_showSmoothPaths()
{
    for (int i = 0; i < _paths.size(); ++i)
    {
        auto blue = 125 - (i * 255/ _paths.size());
        auto green = blue;
        auto red = 255;
        cv::Scalar color(blue, green, red);
        _paths[i]->drawSmoothPath(color);
    }
}

void AppQt::_exportSmoothPath()
{
    _path->writeToFile();
    std::cout << "Smoothed path was exported to the output file <" << _output_file_name << ">\n";
}
