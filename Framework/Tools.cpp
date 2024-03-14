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

	float DistancePlane(const sf::Vector3f& plane, const sf::Vector3f& point)
	{
		return abs(tool::OnPlane(plane,point)) / sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
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
		while (!numStr.empty())
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


	float RadianToDegree(float radian)
	{
		return radian / M_PI * 180.f;
	}
	float Angle(const sf::Vector2f& vec)
	{
		return atan2f(vec.y, vec.x);
	}

	float Angle360(const sf::Vector2f& vec)
	{
		float angle1 = acosf(vec.x / tool::Magnitude(vec));
		if (vec.y < 0)
			angle1 = 2.f * M_PI - angle1;
		return RadianToDegree(angle1);
	}
	float Angle360(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
	{
		return RadianToDegree(acosf((vec1.x * vec2.x + vec1.y * vec2.y) / (Magnitude(vec1) * Magnitude(vec2))));
	}

	float Angle360(const sf::Vector2f& vec1, float degree)
	{
		sf::Vector2f vec2 = { 1.f, 0.f };
		vec2 = sf::Transform().rotate(degree).translate(vec2).transformPoint(vec2);
		return Angle360(vec1, vec2);
	}

	float OnPlane(const sf::Vector3f& plane, const sf::Vector3f& target)
	{
		return plane.x * target.x + plane.y * target.y + plane.z * target.z
			+ sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
	}


	float Clamp(float target, float a, float b)
	{
		if (target > std::max(a, b))
			target = std::max(a, b);
		else if (target < std::min(a, b))
			target = std::min(a, b);
		return target;
	}

	sf::Vector3f To3D(const sf::Vector2f& point)
	{
		return sf::Vector3f(point.x, point.y, 0.f);
	}













}