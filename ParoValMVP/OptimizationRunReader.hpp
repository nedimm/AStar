#pragma once
#include "ParovalParams.hpp"
#include <vector>

class OptimizationRunReader
{
public:
    std::vector<ParovalParams> readOptimizationRun();
};

