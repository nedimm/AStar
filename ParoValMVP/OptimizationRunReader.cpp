#include "OptimizationRunReader.hpp"
#include <fstream>

std::vector<ParovalParams> OptimizationRunReader::readOptimizationRun()
{
    std::vector<ParovalParams> to_return;
    std::ifstream infile("OptimizationRun.txt");
    float obstacle_cost_factor, smooth_rate_alpha, smooth_rate_beta, fitness_value;
    while (infile >> obstacle_cost_factor >> smooth_rate_alpha >> smooth_rate_beta >> fitness_value)
    {
        to_return.push_back({ obstacle_cost_factor, smooth_rate_alpha, smooth_rate_beta });
    }
    return to_return;
}
