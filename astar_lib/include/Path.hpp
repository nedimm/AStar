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
    void createSmoothPath();
    void drawSmoothPath();
    void writeToFile();


protected:
	cv::Mat& _canvas;
	std::shared_ptr<Graph> _graph;
	std::vector<int> _path;
    std::vector<std::vector<float>> _smooth_path;
	cv::Scalar _path_color = cv::Scalar(128, 128, 128);
    cv::Scalar _smooth_path_color = cv::Scalar(0, 0, 255);

	int _thickness = 2;
    float _smooth_rate_alpha = 0.2f;
    float _smooth_rate_beta = 0.2f;
    float _tolerance = 0.000001f;

    bool _should_draw_path = true;
    bool _should_draw_smooth_path = true;
};