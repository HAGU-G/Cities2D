#include "pch.h"
#include "Tools.h"
namespace tool
{
    extern float RandomBetween(float a, float b)
    {
        return std::min(a, b) + (float)rand() / RAND_MAX * (std::max(a, b) - std::min(a, b));
    }
}