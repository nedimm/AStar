#pragma once
struct MVPParams
{
    cv::String mapFileName;
    bool showGrid;
    int gridCellSize;
    float obstacleCostFactor;
    bool showGraph;
    bool showNodeText;
    bool showNodeCosts;
    float smoothAlpha;
    float smoothBeta;
    bool randomStartEnd;
    int startPoint;
    int endPoint;
};
