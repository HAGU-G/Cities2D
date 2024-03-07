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


	float Magnitude(const sf::Vector2f& vec)
	{
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}

	sf::Vector2f GetNormalize(const sf::Vector2f& vec)
	{
		float mag = Magnitude(vec);
		if (mag == 0.f)
			return { 0, 0 };
		return vec / mag;
	}
	void Normalize(sf::Vector2f& vec)
	{
		float mag = Magnitude(vec);
		if (mag != 0.f)
		{
			vec /= mag;
		}
	}

	float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		return Magnitude(p2 - p1);
	}
}