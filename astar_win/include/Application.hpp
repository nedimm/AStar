#pragma once
#include "Map.hpp"
#include <memory>
#include "Grid.hpp"
#include "Graph.hpp"
#include "ParameterServer.hpp"
#include "Path.hpp"

class Application
{
public:
    void start();
    void readParametersFromCSV();
    void readParametersFromCommandLine(char** argv);


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
    bool _show_visualization = false;
    int _grid_cell_size = 10;
    float _obstacle_cost_factor = 1.f;
    float _smooth_rate_alpha = 0.3f;
    float _smooth_rate_beta = 0.2f;
    cv::String _output_file_name = "astar_path.txt";

    // think about parametrizing of this
	// float _astar_movement_cost;
	// number of neighbors in astar (right now 4)
	// changing heuristics (right now using Manhattan)

    bool _initialized = false;
    std::shared_ptr <Map> _map;
    std::shared_ptr <Grid> _grid;
    std::shared_ptr<Graph> _graph;
	std::shared_ptr<Path> _path;
	ParameterServer _parameter_server;
	bool _gridShouldBeShown = true;
};


