#pragma once
#include "ObjectButton.h"

class ButtonNameTag : public ObjectButton
{
protected:
	sf::Text text;

	sf::RectangleShape background;
	sf::CircleShape backgroundCircle;

public:
	explicit ButtonNameTag(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath);
	ButtonNameTag(const ButtonNameTag&) = delete;
	ButtonNameTag(ButtonNameTag&&) = delete;
	ButtonNameTag& operator=(const ButtonNameTag&) = delete;
	ButtonNameTag& operator=(ButtonNameTag&&) = delete;

	void Init() override;
	void Draw(sf::RenderWindow& window) override;
	void SetPosition(const sf::Vector2f& position) override;
	void SetString(const std::string& str);
	void SetString(const std::wstring& wstr);
	void SetOrigin(const sf::Vector2f& origin) override;
	void SetOrigin(ORIGIN origin) override;
	void SetWidth(float width);

	static std::shared_ptr<ButtonNameTag> Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::string& text = "");
	static std::shared_ptr<ButtonNameTag> Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::wstring& text);

};

