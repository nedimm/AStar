#include "../include/Path.hpp"
#include <fstream>

void Path::drawPath() {
    if (_should_draw_path == false) return;
	for (int i = 0; i < _path.size()-1; ++i) {
		auto start = _graph->getNodeFromIndex_1d(_path[i]);
		auto end = _graph->getNodeFromIndex_1d(_path[i+1]);
		cv::line(_canvas, cv::Point(start->x_pos, start->y_pos), cv::Point(end->x_pos, end->y_pos), _path_color, _thickness);
	}
    cv::imshow("A* Visualization", _canvas);
    cv::waitKey(1);
}

void Path::createSmoothPath()
{
    std::vector<std::vector<float>> original_path;
    for (int i = 0; i < _path.size(); ++i)
    {
        std::vector<float> point;
        auto node = _graph->getNodeFromIndex_1d(_path[i]);
        point.push_back(node->x_pos);
        point.push_back(node->y_pos);
        _smooth_path.push_back(point);
        original_path.push_back(point);
    }

    auto change = _tolerance;
    while (change >= _tolerance)
    {
        change = 0.0;
        for (int i = 1; i < _path.size()-1; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                auto aux = _smooth_path[i][j];
                _smooth_path[i][j] += _smooth_rate_alpha * (original_path[i][j] - _smooth_path[i][j]);
                _smooth_path[i][j] += _smooth_rate_beta * (_smooth_path[i - 1][j] + _smooth_path[i + 1][j] - (2.0 * _smooth_path[i][j]));
                change += abs(aux - _smooth_path[i][j]);
            }
        }    
    }
}

void Path::drawSmoothPath()
{
    if (_should_draw_smooth_path == false) return;

    for (int i = 0; i < _smooth_path.size() - 1; ++i) {
        auto start = cv::Point(_smooth_path[i][0], _smooth_path[i][1]);
        auto end = cv::Point(_smooth_path[i+1][0], _smooth_path[i+1][1]);
        cv::line(_canvas, start, end, _smooth_path_color, _thickness);
    }
    cv::imshow("A* Visualization", _canvas);
    cv::waitKey(1);
}

void Path::writeToFile()
{
    std::ofstream output_file("astar_path.txt");
    if (output_file.is_open())
    {
        output_file << "[";
        for (auto point : _smooth_path)
        {
            output_file << "[" << point[0] << "," << point[1] << "]\n";
        }
        output_file.close();
        output_file << "]";
    }
    else std::cout << "Unable to open file";
}
