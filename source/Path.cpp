#include "../include/Path.hpp"

void Path::drawPath() {
	for (int i = 0; i < _path.size()-1; ++i) {
		auto start = _graph->getNodeFromIndex_1d(_path[i]);
		auto end = _graph->getNodeFromIndex_1d(_path[i+1]);
		cv::line(_canvas, cv::Point(start->x_pos, start->y_pos), cv::Point(end->x_pos, end->y_pos), _path_color, _thickness);
	}
    cv::imshow("A* Visualization", _canvas);
    cv::waitKey(0);
}
