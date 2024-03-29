#pragma once
#include <opencv2/core/cvstd.hpp>
#include "Map.hpp"
#include "Grid.hpp"
#include "../astar_win/include/ParameterServer.hpp"
#include "Graph.hpp"
#include "Path.hpp"
#include "MVPParams.hpp"
#include "OptimizationRunReader.hpp"


struct MVPParams;

class AppQt
{
public:
    void setParameters(const MVPParams& params);
    void _showOptimizationRun();
    void visualizeOptimizationRun();
    MVPParams getParameters();
    void displayEnvironment();
    int getLastNodeIndex();
    void start();
    void close() const;

protected:
    void _createCanvas();
    void _showCanvas() const;


    void _createGrid();
    void _showGrid() const;
    void _createGraph();
    void _showGraph() const;
    void _loadMap();
    void _showMap() const;
    void _createAStarPath();
    void _createAStarPaths();

    void _showAStarPath() const;
    void _createSmoothPath();
    void _createSmoothPaths();

    void _showSmoothPath();
    void _showSmoothPaths();

    void _exportSmoothPath();

    cv::String _params_file_name = "parameters.csv";

    MVPParams _params{
        "testmap_883_556.png",
    true,
        40,
        1.f,
        true,
        false,
        true,
        0.1f,
        0.3f,
        true,
        0,
        0
    };
    // parameters
    cv::String _output_file_name = "astar_path.txt";

    bool _initialized = false;
    std::shared_ptr <Map> _map = nullptr;
    std::shared_ptr <Grid> _grid = nullptr;
    std::shared_ptr<Graph> _graph = nullptr;
    std::shared_ptr<Path> _path = nullptr;
    bool _pathFindingIsRunning = false;

    OptimizationRunReader _optimization_run_reader;
    std::vector<std::shared_ptr<Path>> _paths;
    std::vector<ParovalParams> _paroval_params;
};
