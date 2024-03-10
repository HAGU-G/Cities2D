#include "pch.h"
#include "SceneMenu.h"
#include <ObjectButton.h>
#include <ButtonNineSlice.h>

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
	SFGM_TEXTURE.Add("resource/ui/optionOn.png");
	SFGM_TEXTURE.Add("resource/ui/optionDown.png");
	SFGM_TEXTURE.Add("resource/ui/new.png");
	SFGM_TEXTURE.Add("resource/ui/newOn.png");
	SFGM_TEXTURE.Add("resource/ui/newDown.png");
	SFGM_TEXTURE.Add("resource/ui/continue.png");
	SFGM_TEXTURE.Add("resource/ui/continueOn.png");
	SFGM_TEXTURE.Add("resource/ui/continueDown.png");
	SFGM_TEXTURE.Add("resource/ui/exit.png");
	SFGM_TEXTURE.Add("resource/ui/exitOn.png");
	SFGM_TEXTURE.Add("resource/ui/exitDown.png");
	SFGM_TEXTURE.Add("resource/ui/load.png");
	SFGM_TEXTURE.Add("resource/ui/loadOn.png");
	SFGM_TEXTURE.Add("resource/ui/loadDown.png");

	SFGM_FONT.Add("resource/font/ROKAF Sans Medium.ttf");

	SFGM_SOUNDBUFFER.Add("resource/sfx/InMouse.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/Click.wav");
	SFGM_SOUNDBUFFER.Add("resource/music/LittleClose.wav");
}

void SceneMenu::Init()
{
	Scene::Init();

	menuBgm.setBuffer(SFGM_SOUNDBUFFER.Get("resource/music/LittleClose.wav"));
	menuBgm.setLoop(true);
	menuBgm.play();


	ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(-452.5f, 0.f),
		"continue", L"게임 계속");
	ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(-452.5f, 85.f),
		"new", L"새 게임", std::bind(&SceneMenu::New, this));
	ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(-452.5f, 170.f),
		"option", L"옵션");
	ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(-452.5f, 255.f),
		"exit", L"종료", std::bind(&GameManager::Exit));
	std::shared_ptr<ButtonNineSlice> load = ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(2.5f, 0.f),
		"load", L"불러오기");
	

	load->SetSize({ 450.f,335.f });

}

void SceneMenu::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneMenu::Continue()
{
}

void SceneMenu::New()
{
	SceneManager::Use("SceneGame");
	SceneManager::Use("SceneGameUI");
	SceneManager::Unuse("SceneMenu");
	SceneManager::Unuse("SceneTitle");
	menuBgm.stop();
}

void SceneMenu::Option()
{
}
