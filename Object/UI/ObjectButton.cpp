#include "pch.h"
#include "ObjectButton.h"

ObjectButton::ObjectButton(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconName, const std::function<void()>& func)
	:GameObject(scene, GAME_OBJECT_TYPE::BUTTON), funcVoid_void(func), iconName(iconName)
{
	this->position = position;

}

void ObjectButton::Init()
{
	SetIcon(iconName);
	icon.setOrigin(0.f, icon.getLocalBounds().height * 0.5f);
	icon.setScale(0.25f, 0.25f);



	mouseOn.setBuffer(SFGM_SOUNDBUFFER.Get("resource/sfx/InMouse.wav"));
	click.setBuffer(SFGM_SOUNDBUFFER.Get("resource/sfx/Click.wav"));

	SetPosition(position);
}

void ObjectButton::Update(float timeDelta, float timeScale)
{
	switch (state)
	{
	case 0:
		if (bound.contains(scene.lock()->GetMousePosWorld()))
		{
			SetState(1);
			mouseOn.play();
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
			if (bound.contains(scene.lock()->GetMousePosWorld()))
			{
				if (doToggle)
					isSelect = !isSelect;

				if (!isOnlyDown)
				{
					SetState(1);
				}
				if (funcVoid_void != nullptr)
					funcVoid_void();
				OnDown();

			}
			else if(!isOnlyDown)
			{
				SetState(0);
			}
		}
		break;
	default:
		break;
	}
}

void ObjectButton::Draw(sf::RenderWindow& window)
{
	window.draw(icon);
}

void ObjectButton::SetOrigin(ORIGIN origin)
{
	sf::Vector2f newOrigin(icon.getLocalBounds().width, icon.getLocalBounds().height);
	newOrigin.x *= ((int)origin % 3) * 0.5f;
	newOrigin.y *= ((int)origin / 3) * 0.5f;
	SetOrigin(newOrigin);
}

void ObjectButton::SetOrigin(const sf::Vector2f& origin)
{
	this->origin = origin;
	icon.setOrigin(this->origin);
	bound = icon.getGlobalBounds();
}



void ObjectButton::SetState(int state)
{

	switch (state)
	{
	case 0:
		this->state = state;
		if (isSelect)
		{
			SetIcon(iconName + "Down");
		}
		else
		{
			SetIcon(iconName);
		}
		break;
	case 1:
		this->state = state;
		SetIcon(iconName + "On");
		break;
	case 2:
		this->state = state;
		SetIcon(iconName + "Down");
		break;
	default:
		break;
	}
}

void ObjectButton::SetPosition(const sf::Vector2f& position)
{
	this->position = position;
	icon.setPosition(position);
	bound = icon.getGlobalBounds();
}


void ObjectButton::SetIcon(const std::string& iconName)
{
	icon.setTexture(SFGM_TEXTURE.Get("resource/ui/" + iconName + ".png"));
}

void ObjectButton::UnSelect()
{
	isSelect = false;
	SetState(0);
}

void ObjectButton::Select()
{
	isSelect = true;
	SetState(2);
}

void ObjectButton::OnDown()
{
}

std::shared_ptr<ObjectButton> ObjectButton::Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath,
	const std::function<void()>& func, bool toggle)
{
	std::shared_ptr<ObjectButton> objectButton = std::make_shared<ObjectButton>(scene, position, iconPath, func);
	scene.lock()->AddObject(objectButton);
	objectButton->Init();
	objectButton->Reset();
	objectButton->doToggle = toggle;
	return objectButton;
}