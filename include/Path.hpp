#pragma once
#include <opencv2/opencv.hpp>
#include "Node.hpp"
#include "Graph.hpp"

class Path {
public:
	Path(cv::Mat& canvas, std::vector<int> path, std::shared_ptr<Graph> graph):
		_canvas(canvas),
		_path(path),
		_graph(graph){}

	void drawPath();

protected:
	cv::Mat& _canvas;
	std::shared_ptr<Graph> _graph;
	std::vector<int> _path;
	cv::Scalar _path_color = cv::Scalar(0, 0, 255);
	int _thickness = 2;

};