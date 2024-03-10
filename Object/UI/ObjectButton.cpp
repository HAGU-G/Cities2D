#include "pch.h"
#include "ObjectButton.h"

ObjectButton::ObjectButton(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::string& str, const std::function<void()>& func)
	:GameObject(scene, GAME_OBJECT_TYPE::BUTTON), func(func)
{
	this->position = position;
	text.setString(str);
	SetIcon(iconPath);
}

ObjectButton::ObjectButton(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::wstring& str, const std::function<void()>& func)
	:GameObject(scene, GAME_OBJECT_TYPE::BUTTON), func(func)
{
	this->position = position;
	text.setString(str);
	SetIcon(iconPath);
}


void ObjectButton::Init()
{
	SetPosition(position);
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

	icon.setOrigin(0.f, icon.getLocalBounds().height * 0.5f);
	icon.setScale(0.25f, 0.25f);

	ContentsUpdate();

	text.setCharacterSize(30);
	text.setFont(SFGM_FONT.Get("resource/font/ROKAF Sans Medium.ttf"));
	text.setFillColor(sf::Color::White);
	text.setOrigin(0.f, text.getCharacterSize() * 0.7f);


	SetSize({ 450.f, 80.f });

	hover.setBuffer(SFGM_SOUNDBUFFER.Get("resource/sfx/InMouse.wav"));
	click.setBuffer(SFGM_SOUNDBUFFER.Get("resource/sfx/Click.wav"));
}

void ObjectButton::Update(float timeDelta, float timeScale)
{
	switch (state)
	{
	case 0:
		if (bound.contains(scene.lock()->GetMousePosWorld()))
		{
			SetState(1);
			hover.play();
		}
		break;
	case 1:
		if (!bound.contains(scene.lock()->GetMousePosWorld()))
		{
			SetState(0);
		}
		else if (IOManager::IsKeyDown(sf::Mouse::Left))
		{
			SetState(2);
			click.play();
		}
		break;
	case 2:
		if (IOManager::IsKeyUp(sf::Mouse::Left))
		{
			SetState(0);
			if (bound.contains(scene.lock()->GetMousePosWorld()))
			{
				if (func != nullptr)
					func();
			}
		}
		break;
	default:
		break;
	}
}

void ObjectButton::Draw(sf::RenderWindow& window)
{
	window.draw(nineSlicing, renderStates);
	window.draw(icon);
	window.draw(text);
}

void ObjectButton::SetOrigin(ORIGIN origin)
{
	sf::Vector2f newOrigin(nineSlicing.getBounds().width, nineSlicing.getBounds().height);
	newOrigin.x *= ((int)origin % 3) * 0.5f;
	newOrigin.y *= ((int)origin / 3) * 0.5f;
	SetOrigin(newOrigin);
	ContentsUpdate();
}

void ObjectButton::SetString(const std::string& str)
{
	text.setString(str);
}
void ObjectButton::SetString(const std::wstring& wstr)
{
	text.setString(wstr);
}

void ObjectButton::ContentsUpdate()
{
	renderStates.transform = sf::Transform().scale(scale, position).rotate(rotate, position).translate(position - origin);

	bound.left = position.x - origin.x * scale.x;
	bound.top = position.y - origin.y * scale.y;
	bound.width = nineSlicing.getBounds().width * scale.x;
	bound.height = nineSlicing.getBounds().height * scale.y;

	icon.setPosition(bound.left, bound.top + bound.height * 0.5f);
	text.setPosition(bound.left + icon.getGlobalBounds().width, bound.top + bound.height * 0.5f);
}

void ObjectButton::SetState(int state)
{

	switch (state)
	{
	case 0:
		this->state = state;
		renderStates.texture = &SFGM_TEXTURE.Get("resource/ui/button.png");
		break;
	case 1:
		this->state = state;
		renderStates.texture = &SFGM_TEXTURE.Get("resource/ui/buttonOn.png");
		break;
	case 2:
		this->state = state;
		renderStates.texture = &SFGM_TEXTURE.Get("resource/ui/buttonDown.png");
		break;
	default:
		break;
	}
}

void ObjectButton::SetOrigin(sf::Vector2f origin)
{
	this->origin = origin;
	ContentsUpdate();
}

void ObjectButton::SetPosition(const sf::Vector2f& position)
{
	this->position = position;
	ContentsUpdate();
}


void ObjectButton::SetSize(sf::Vector2f size)
{


	for (int i = 0; i < 9; i++)
	{
		nineSlicing[i * 4 + 0].position = { (i % 3) * (textureSize.x / 3.f), (i / 3) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 1].position = { (i % 3 + 1) * (textureSize.x / 3.f), (i / 3) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 2].position = { (i % 3 + 1) * (textureSize.x / 3.f), (i / 3 + 1) * (textureSize.y / 3.f) };
		nineSlicing[i * 4 + 3].position = { (i % 3) * (textureSize.x / 3.f), (i / 3 + 1) * (textureSize.y / 3.f) };
	}
	sf::Vector2f delta;
	delta.x= size.x - nineSlicing.getBounds().width * scale.x;
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



void ObjectButton::SetIcon(const std::string& path)
{
	icon.setTexture(SFGM_TEXTURE.Get(path));
}