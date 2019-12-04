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

protected:
	void _readParameters();
    void _showMainWindow();
    void _createGrid();
    void _showGrid();

    void _createGraph();
    void _showGraph();
    void _loadMap();
    void _showMap();
    void _runAStar();
	void _showPath();
    void _showSmoothPath();


	// think about parametrization of this:
    int _grid_cell_size = 10;
	// float _astar_movement_cost;
	// number of neighbors in astar (right now 4)
	// changing heuristics (right now using Manhattan)
	
    std::shared_ptr <Map> _map;
    std::shared_ptr <Grid> _grid;
    std::shared_ptr<Graph> _graph;
	std::shared_ptr<Path> _path;
	ParameterServer _parameter_server;
	bool _gridShouldBeShown = false;
};


