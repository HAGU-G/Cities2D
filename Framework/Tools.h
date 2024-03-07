#pragma once
#ifndef TOOLS_H
#define TOOLS_H

namespace tool {
	float RandomBetween(float a, float b);
	int RandomBetween(int a, int b);

	float Magnitude(const sf::Vector2f& vec);
	sf::Vector2f GetNormalize(const sf::Vector2f& vec);
	void Normalize(sf::Vector2f& vec);
	float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2);
}
#endif // !TOOLS_H