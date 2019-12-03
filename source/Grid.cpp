#include "../include/Grid.hpp"
#include <opencv2/video.hpp>

Grid::Grid(int width, int height, int cell_size):
_width(width)
, _height(height)
, _cell_size(cell_size)
{
}

void Grid::drawGrid(cv::Mat& image_to_draw_on)
{
    for (int i = 0; i < _height; i += _cell_size)
    {
        cv::line(image_to_draw_on, cv::Point(0, i), cv::Point(_width, i), _color, _thickness, _line_type);
    }

    for (int i = 0; i < _width; i += _cell_size)
    {
        cv::line(image_to_draw_on, cv::Point(i,0), cv::Point(i, _height), _color, _thickness, _line_type);
    }
}
