#pragma once
#ifndef TOOLS_H
#define TOOLS_H

enum class ORIGIN
{
	LT, CT, RT,
	LC, CC, RC,
	LB, CB, RB,

	TL = LT, TR = RT, BL = LB, BR = RB,
	TC = CT, MT = CT, TM = CT,
	CL = LC, ML = LC, LM = LC,
	CM = CC, MC = CC, MM = CC, C = CC, M = CC,
	CR = RC, MR = RC, RM = RC,
	BC = CB, MB = CB, BM = CB
};

namespace tool {
	//distance
	float Magnitude(const sf::Vector2f& vec);
	sf::Vector2f GetNormalize(const sf::Vector2f& vec);
	void Normalize(sf::Vector2f& vec);
	float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

	//Origin
	sf::Vector2f SetOrigin(sf::Transformable& transformable, ORIGIN origin, const sf::FloatRect& rect);
	sf::Vector2f SetOrigin(sf::Sprite& sprite, ORIGIN origin);
	sf::Vector2f SetOrigin(sf::Text& text, ORIGIN origin);
	sf::Vector2f SetOrigin(sf::Shape& shape, ORIGIN origin);

	//conversion
	std::string ThousandsSeparator(int num);

	//Angle
	float RadianToDegree(float radian);
	float Angle(const sf::Vector2f& vec);
	float Angle360(const sf::Vector2f& vec);
	float Angle360(const sf::Vector2f& vec1, const sf::Vector2f& vec2);
	float Angle360(const sf::Vector2f& vec1, float degree);

	//clamp
	float Clamp(float target, float a, float b);

	//3D
	float OnPlane(const sf::Vector3f& plane, const sf::Vector3f& target);
	sf::Vector3f To3D(const sf::Vector2f& point);
}
#endif // !TOOLS_H