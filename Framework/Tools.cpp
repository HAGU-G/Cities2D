#include "pch.h"
#include "Tools.h"
namespace tool
{
	float RandomBetween(float a, float b)
	{
		return std::min(a, b) + (float)rand() / RAND_MAX * (std::max(a, b) - std::min(a, b));
	}
	int RandomBetween(int a, int b)
	{
		return std::min(a, b) + rand() % (std::max(a, b) - std::min(a, b) + 1);
	}
}