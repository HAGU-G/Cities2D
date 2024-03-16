#include "pch.h"
#include "ButtonNameTag.h"

ButtonNameTag::ButtonNameTag(std::weak_ptr<Scene> scene, sf::Vector2f position,
	const std::string& iconPath, const std::function<void()>& func)
	:ObjectButton(scene, position, iconPath, func)
{
}

void ButtonNameTag::Init()
{
	ObjectButton::Init();

	background.setSize(icon.getGlobalBounds().getSize());
	tool::SetOrigin(background, ORIGIN::LC);
	backgroundCircle.setRadius(icon.getGlobalBounds().height * 0.5f);
	tool::SetOrigin(backgroundCircle, ORIGIN::C);

	sf::Image image = icon.getTexture()->copyToImage();
	color = image.getPixel(image.getSize().x, image.getSize().y / 2);
	background.setFillColor(color);
	backgroundCircle.setFillColor(color);

	text.setCharacterSize(backgroundCircle.getRadius() * 0.75f);
	text.setFont(SFGM_FONT.Get("resource/font/ROKAF Sans Medium.ttf"));
	text.setFillColor(sf::Color::Black);
	SetPosition(position);
}

void ButtonNameTag::Update(float timeDelta, float timeScale)
{
	ObjectButton::Update(timeDelta, timeScale);
	if (isInputMode)
	{
		if (IOManager::IsDoInputText())
		{
			SetString(IOManager::GetInputText());
		}
		else
		{
			if (funcVoid_void != nullptr)
				funcVoid_void();
			IOManager::ClearInputText();
			isInputMode = false;
			UnSelect();
		}
	}

	sf::Image image = icon.getTexture()->copyToImage();
	color = image.getPixel(image.getSize().x, image.getSize().y / 2);
	background.setFillColor(color);
	backgroundCircle.setFillColor(color);
}

void ButtonNameTag::Draw(sf::RenderWindow& window)
{
	window.draw(background);
	window.draw(backgroundCircle);
	window.draw(text);
	ObjectButton::Draw(window);

}

void ButtonNameTag::SetPosition(const sf::Vector2f& position)
{
	ObjectButton::SetPosition(position);
	sf::FloatRect iconRect = icon.getGlobalBounds();
	background.setPosition(iconRect.left + iconRect.width * 0.5f, iconRect.top + iconRect.height * 0.5f);

	sf::FloatRect backRect = background.getGlobalBounds();
	backgroundCircle.setPosition(backRect.left + backRect.width, backRect.top + backRect.height * 0.5f);
	text.setOrigin(text.getGlobalBounds().width, text.getCharacterSize() * 0.7f);
	text.setPosition(backgroundCircle.getPosition());
	bound = { iconRect.left,iconRect.top,iconRect.width + backRect.width,iconRect.height };
}

void ButtonNameTag::SetString(const std::string& str)
{
	text.setString(str);
	SetPosition(position);
}

void ButtonNameTag::SetString(const std::wstring& wstr)
{
	text.setString(wstr);
	SetPosition(position);
}

std::shared_ptr<ButtonNameTag> ButtonNameTag::Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::string& text, const std::function<void()>& func)
{
	std::shared_ptr<ButtonNameTag> buttonNameTag = std::make_shared<ButtonNameTag>(scene, position, iconPath, func);
	scene.lock()->AddObject(buttonNameTag);
	buttonNameTag->Init();
	buttonNameTag->Reset();
	buttonNameTag->SetString(text);
	return buttonNameTag;
}

std::shared_ptr<ButtonNameTag> ButtonNameTag::Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::wstring& text, const std::function<void()>& func)
{
	std::shared_ptr<ButtonNameTag> buttonNameTag = std::make_shared<ButtonNameTag>(scene, position, iconPath, func);
	scene.lock()->AddObject(buttonNameTag);
	buttonNameTag->Init();
	buttonNameTag->Reset();
	buttonNameTag->SetString(text);
	return buttonNameTag;
}

void ButtonNameTag::SetOrigin(const sf::Vector2f& origin)
{
	this->origin = origin;
	float buttonWidth = icon.getGlobalBounds().width + background.getGlobalBounds().width;
	icon.setOrigin(buttonWidth * origin.x / icon.getGlobalBounds().width, this->origin.y);
	bound = { icon.getGlobalBounds().left,icon.getGlobalBounds().top,buttonWidth,icon.getGlobalBounds().height };
	SetPosition(position);
}

void ButtonNameTag::SetOrigin(ORIGIN origin)
{
	ObjectButton::SetOrigin(origin);
}

void ButtonNameTag::SetWidth(float width)
{
	background.setSize({ width - icon.getGlobalBounds().width, background.getSize().y });
	SetOrigin(origin);
}

void ButtonNameTag::SetInputMode(bool value, size_t textSize)
{
	isInputMode = value;
	IOManager::SetDoInputText(isInputMode, textSize);
}

void ButtonNameTag::OnDown()
{

	ObjectButton::OnDown();
	if (isTextField)
		SetInputMode(!isInputMode);

}
