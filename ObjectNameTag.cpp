#include "pch.h"
#include "ObjectNameTag.h"

ButtonNameTag::ButtonNameTag(std::weak_ptr<Scene> scene, sf::Vector2f position, 
	const std::string& iconPath,const std::string& text)
	:ObjectButton(scene,position,iconPath), text(text)
{
}
