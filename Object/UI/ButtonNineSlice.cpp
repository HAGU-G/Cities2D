#include "pch.h"
#include "ButtonNineSlice.h"

ButtonNineSlice::ButtonNineSlice(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconName, const std::string& str, const std::function<void()>& func)
	:ObjectButton(scene, position, iconName, func)
{
	scale = { 0.5f,0.5f };
	text.setString(str);
}

ButtonNineSlice::ButtonNineSlice(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconName, const std::wstring& str, const std::function<void()>& func)
	:ObjectButton(scene, position, iconName, func)
{
	scale = { 0.5f,0.5f };
	text.setString(str);
}


void ButtonNineSlice::Init()
{
	ObjectButton::Init();

	text.setCharacterSize(30);
	text.setFont(SFGM_FONT.Get("resource/font/ROKAF Sans Medium.ttf"));
	text.setFillColor(sf::Color::White);

	text.setOrigin(0.f, text.getCharacterSize() * 0.7f);
	renderStates.texture = &SFGM_TEXTURE.Get("resource/ui/button.png");
	textureSize = renderStates.texture->getSize();
	nineSlicing.setPrimitiveType(sf::Quads);
	nineSlicing.resize(36);
	for (int i = 0; i < 9; i++)
	{
		nineSlicing[i * 4 + 0].texCoords = { (i % 3) * (textureSize.x / 3.f), (i / 3) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 1].texCoords = { (i % 3 + 1) * (textureSize.x / 3.f), (i / 3) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 2].texCoords = { (i % 3 + 1) * (textureSize.x / 3.f), (i / 3 + 1) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 3].texCoords = { (i % 3) * (textureSize.x / 3.f), (i / 3 + 1) * (textureSize.y / 3.f) };

		nineSlicing[i * 4 + 0].color = { color };
		nineSlicing[i * 4 + 1].color = { color };
		nineSlicing[i * 4 + 2].color = { color };
		nineSlicing[i * 4 + 3].color = { color };

		nineSlicing[i * 4 + 0].position = { (i % 3) * (textureSize.x / 3.f), (i / 3) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 1].position = { (i % 3 + 1) * (textureSize.x / 3.f), (i / 3) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 2].position = { (i % 3 + 1) * (textureSize.x / 3.f), (i / 3 + 1) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 3].position = { (i % 3) * (textureSize.x / 3.f), (i / 3 + 1) * (textureSize.y / 3.f) };
	}
	ContentsUpdate();
	SetSize({ 450.f, 80.f });
}

void ButtonNineSlice::Draw(sf::RenderWindow& window)
{
	window.draw(nineSlicing, renderStates);
	ObjectButton::Draw(window);
	window.draw(text);
}

void ButtonNineSlice::SetOrigin(ORIGIN origin)
{
	sf::Vector2f newOrigin(nineSlicing.getBounds().width, nineSlicing.getBounds().height);
	newOrigin.x *= ((int)origin % 3) * 0.5f;
	newOrigin.y *= ((int)origin / 3) * 0.5f;
	ObjectButton::SetOrigin(origin);
	ContentsUpdate();
}

void ButtonNineSlice::OnDown()
{
	if (funcVoid_string != nullptr)
		funcVoid_string(text.getString());
	ObjectButton::OnDown();
}

void ButtonNineSlice::ContentsUpdate()
{
	renderStates.transform = sf::Transform().scale(scale, position).rotate(rotate, position).translate(position - origin);

	bound.left = position.x - origin.x * scale.x;
	bound.top = position.y - origin.y * scale.y;
	bound.width = nineSlicing.getBounds().width * scale.x;
	bound.height = nineSlicing.getBounds().height * scale.y;

	icon.setPosition(bound.left, bound.top + bound.height * 0.5f);
	text.setPosition(bound.left + icon.getGlobalBounds().width, bound.top + bound.height * 0.5f);
}

void ButtonNineSlice::SetState(int state)
{
	switch (state)
	{
	case 0:
		renderStates.texture = &SFGM_TEXTURE.Get("resource/ui/button.png");
		break;
	case 1:
		renderStates.texture = &SFGM_TEXTURE.Get("resource/ui/buttonOn.png");
		break;
	case 2:
		renderStates.texture = &SFGM_TEXTURE.Get("resource/ui/buttonDown.png");
		break;
	default:
		break;
	}
	ObjectButton::SetState(state);
}

void ButtonNineSlice::SetPosition(const sf::Vector2f& position)
{
	this->position = position;
	ContentsUpdate();
}


void ButtonNineSlice::SetSize(sf::Vector2f size)
{
	for (int i = 0; i < 9; i++)
	{
		nineSlicing[i * 4 + 0].position = { (i % 3) * (textureSize.x / 3.f), (i / 3) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 1].position = { (i % 3 + 1) * (textureSize.x / 3.f), (i / 3) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 2].position = { (i % 3 + 1) * (textureSize.x / 3.f), (i / 3 + 1) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 3].position = { (i % 3) * (textureSize.x / 3.f), (i / 3 + 1) * (textureSize.y / 3.f) };
	}
	sf::Vector2f delta;
	delta.x = size.x - nineSlicing.getBounds().width * scale.x;
	delta.y = size.y - nineSlicing.getBounds().height * scale.y;
	sf::Vector2f halfPos = { nineSlicing.getBounds().width * 0.5f, nineSlicing.getBounds().height * 0.5f };

	for (int i = 0; i < nineSlicing.getVertexCount(); i++)
	{
		if (nineSlicing[i].position.x > halfPos.x)
			nineSlicing[i].position.x += delta.x / scale.x;

		if (nineSlicing[i].position.y > halfPos.y)
			nineSlicing[i].position.y += delta.y / scale.y;
	}

	ContentsUpdate();
}

std::shared_ptr<ButtonNineSlice> ButtonNineSlice::Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath,
	const std::string& str, const std::function<void()>& func)
{
	std::shared_ptr<ButtonNineSlice> buttonNineSlice = std::make_shared<ButtonNineSlice>(scene, position, iconPath, str, func);
	scene.lock()->AddObject(buttonNineSlice);
	buttonNineSlice->Init();
	buttonNineSlice->Reset();
	return buttonNineSlice;
}

std::shared_ptr<ButtonNineSlice> ButtonNineSlice::Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath,
	const std::wstring& str, const std::function<void()>& func)
{
	std::shared_ptr<ButtonNineSlice> buttonNineSlice = std::make_shared<ButtonNineSlice>(scene, position, iconPath, str, func);
	scene.lock()->AddObject(buttonNineSlice);
	buttonNineSlice->Init();
	buttonNineSlice->Reset();
	return buttonNineSlice;
}

void ButtonNineSlice::SetString(const std::string& str)
{
	text.setString(str);
}
void ButtonNineSlice::SetString(const std::wstring& wstr)
{
	text.setString(wstr);
}