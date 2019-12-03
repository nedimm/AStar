#pragma once
#include <opencv2/core/async.hpp>

class Grid
{
public:
    Grid(int width, int height, int cell_size);
    void drawGrid(cv::Mat& image_to_draw_on);

protected:
    int _width;
    int _height;
    int _cell_size;
    cv::Scalar _color = cv::Scalar(109, 184, 210);
    int _thickness = 1;
    int _line_type = 8;
};


