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

	sf::Vector2f SetOrigin(sf::Transformable& transformable, ORIGIN origin, const sf::FloatRect& rect)
	{
		// Rect Width, Height
		sf::Vector2f newOrigin(rect.width, rect.height);
		newOrigin.x *= ((int)origin % 3) * 0.5f; // 0 1 2 => 0 0.5 1
		newOrigin.y *= ((int)origin / 3) * 0.5f; // 0 1 2 => 0 0.5 1
		transformable.setOrigin(newOrigin);

		return newOrigin;
	}
	sf::Vector2f SetOrigin(sf::Sprite& sprite, ORIGIN origin)
	{
		return SetOrigin(sprite, origin, sprite.getLocalBounds());
	}
	sf::Vector2f SetOrigin(sf::Text& text, ORIGIN origin)
	{
		return SetOrigin(text, origin, text.getLocalBounds());
	}
	sf::Vector2f SetOrigin(sf::Shape& shape, ORIGIN origin)
	{
		return SetOrigin(shape, origin, shape.getLocalBounds());
	}

	std::string ThousandsSeparator(int num)
	{
		if (num < 1000)
			return to_string(num);
		std::string numStr = to_string(num);
		std::string newStr;
		int count = 0;
		while(!numStr.empty())
		{
			newStr = numStr.back() + newStr;
			numStr.pop_back();
			count++;
			if (count == 3 && !numStr.empty())
			{
				newStr = "," + newStr;
				count = 0;
			}
		}
		return newStr;
	}













}