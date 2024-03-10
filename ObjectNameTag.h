#pragma once
#include "ObjectButton.h"

class ButtonNameTag : public ObjectButton
{
protected:
	std::string text;

public:
	explicit ButtonNameTag(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::string& text);
	ButtonNameTag(const ButtonNameTag&) = delete;
	ButtonNameTag(ButtonNameTag&&) = delete;
	ButtonNameTag& operator=(const ButtonNameTag&) = delete;
	ButtonNameTag& operator=(ButtonNameTag&&) = delete;
};

