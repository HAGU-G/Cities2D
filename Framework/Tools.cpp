#include "pch.h"
#include "Tools.h"
namespace tool {
    float Random(float a, float b)
    {
        srand(time(NULL));
        return std::min(a, b) + (float)rand() / RAND_MAX * (std::max(a, b) - std::min(a, b));
    }
}