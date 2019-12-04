#include "../include/Map.hpp"
#include <opencv2/imgproc/imgproc.hpp>

Map::Map(cv::String file_path) :
    _file_path(file_path) {  }

void Map::load()
{
    _canvas = cv::imread(cv::samples::findFile(_file_path), cv::IMREAD_COLOR);
    cv::Mat image_gray;
    cv::cvtColor(_canvas, image_gray, cv::COLOR_BGR2GRAY);
    
    cv::Mat imgage_bw;
    // convert gray image to binary image
    cv::threshold(image_gray, imgage_bw, 200, 255.0, cv::THRESH_BINARY);

    // 0 for blocked cell and 1 for free cell
    _image = imgage_bw / 255; // 
    if (_image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
    }
}

cv::Mat& Map::getImage()
{
    return _image;
}

cv::Mat& Map::getCanvas()
{
    return _canvas;
}

int Map::getWidth()
{
    if (_image.empty())
    {
        std::cout << "Image is empty" << std::endl;
    }
    return _image.cols;
}

int Map::getHeight()
{
    if (_image.empty())
    {
        std::cout << "Image is empty" << std::endl;
    }
    return _image.rows;
}
