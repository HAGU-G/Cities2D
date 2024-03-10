#include "pch.h"
#include "SceneMenu.h"
#include <ObjectButton.h>

SceneMenu::SceneMenu(const std::string& name)
	:Scene(name)
{
}

void SceneMenu::AddResource()
{
	SFGM_TEXTURE.Add("resource/ui/button.png");
	SFGM_TEXTURE.Add("resource/ui/buttonOn.png");
	SFGM_TEXTURE.Add("resource/ui/buttonDown.png");
	SFGM_TEXTURE.Add("resource/ui/option.png");
	SFGM_TEXTURE.Add("resource/ui/new.png");
	SFGM_TEXTURE.Add("resource/ui/continue.png");
	SFGM_TEXTURE.Add("resource/ui/exit.png");
	SFGM_TEXTURE.Add("resource/ui/load.png");
	SFGM_FONT.Add("resource/font/ROKAF Sans Medium.ttf");
	SFGM_SOUNDBUFFER.Add("resource/sfx/InMouse.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/Click.wav");
}

void SceneMenu::Init()
{
	AddObject(std::make_shared<ObjectButton>(This(), worldCenter + sf::Vector2f(-452.5f,-80.f),
		"resource/ui/continue.png",L"게임 계속"));
	AddObject(std::make_shared<ObjectButton>(This(), worldCenter + sf::Vector2f(-452.5f,5.f),
		"resource/ui/new.png",L"새 게임"));
	AddObject(std::make_shared<ObjectButton>(This(), worldCenter + sf::Vector2f(-452.5f,90.f),
		"resource/ui/option.png",L"옵션"));
	AddObject(std::make_shared<ObjectButton>(This(), worldCenter + sf::Vector2f(-452.5f,175.f),
		"resource/ui/exit.png",L"종료", std::bind(&GameManager::Exit)));

	std::shared_ptr<ObjectButton> load = std::make_shared<ObjectButton>(This(), worldCenter + sf::Vector2f(2.5f, -80.f),
		"resource/ui/load.png", L"불러오기");
	AddObject(load);

	Scene::Init();
	load->SetSize({ 450.f,335.f });
	view.setCenter(worldCenter);

}

void SceneMenu::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
