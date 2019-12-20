#pragma once
#include <opencv2/core/cvstd.hpp>
#include "Map.hpp"
#include "Grid.hpp"
#include "../astar_win/include/ParameterServer.hpp"
#include "Graph.hpp"
#include "Path.hpp"

struct MVPParams;

class AppQt
{
public:
    void readParameters(const MVPParams& params);
    void start();
    void close();

protected:
    void _createGrid();
    void _showGrid();
    void _createGraph();
    void _showGraph();
    void _loadMap();
    void _showMap();
    void _createAStarPath();
    void _showAStarPath();
    void _createSmoothPath();
    void _showSmoothPath();
    void _exportSmoothPath();
    void _showNodeCosts();

    cv::String _params_file_name = "parameters.csv";

    // parameters
    cv::String _map_file_name = "testmap_883_556.png";
    bool _show_visualization = true;
    int _grid_cell_size = 40;
    float _obstacle_cost_factor = 1.f;
    float _smooth_rate_alpha = 0.3f;
    float _smooth_rate_beta = 0.2f;
    cv::String _output_file_name = "astar_path.txt";

    bool _initialized = false;
    std::shared_ptr <Map> _map;
    std::shared_ptr <Grid> _grid;
    std::shared_ptr<Graph> _graph;
    std::shared_ptr<Path> _path;
    bool _gridShouldBeShown = true;
    bool _running = false;
};
