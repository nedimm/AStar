#pragma once
#include <opencv2/opencv.hpp>

class Map
{
public:
    Map(cv::String file_path);

    void load();
    cv::Mat& getImage();
    cv::Mat& getCanvas();

    int getWidth();
    int getHeight();
protected:
    cv::String _file_path;
    cv::Mat _image;
    cv::Mat _canvas;
};

