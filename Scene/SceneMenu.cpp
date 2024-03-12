#include "pch.h"
#include "SceneMenu.h"
#include <ObjectButton.h>
#include <ButtonNineSlice.h>
#include <SceneGame.h>
#include <SceneGameUI.h>

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
	SFGM_TEXTURE.Add("resource/ui/save.png");
	SFGM_TEXTURE.Add("resource/ui/saveOn.png");
	SFGM_TEXTURE.Add("resource/ui/saveDown.png");
	SFGM_TEXTURE.Add("resource/ui/loading.png");
	SFGM_TEXTURE.Add("resource/ui/loadingOn.png");
	SFGM_TEXTURE.Add("resource/ui/loadingDown.png");

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

	background.setFillColor({ 0,0,0,220 });
	background.setSize(view.getSize());
	tool::SetOrigin(background, ORIGIN::MC);
	background.setPosition(view.getCenter());


	lastGame = ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(-452.5f, 0.f),
		"continue", L"이어하기", std::bind(&SceneMenu::Continue,this));
	playGame = ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(-452.5f, 0.f),
		"continue", L"게임 계속", std::bind(&SceneMenu::GameContinue, this));
	playGame->SetActive(false);
	newGame = ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(-452.5f, 85.f),
		"new", L"새 게임", std::bind(&SceneMenu::New, this));
	saveGame = ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(-452.5f, 85.f),
		"save", L"게임 저장", std::bind(&SceneMenu::Save, this));
	saveGame->SetActive(false);
	ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(-452.5f, 170.f),
		"option", L"옵션");
	ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(-452.5f, 255.f),
		"exit", L"종료", std::bind(&GameManager::Exit));

	resetGame = ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(2.5f, 0.f),
		"loading", L"초기화", std::bind(&SceneMenu::GameReset, this));
	resetGame->SetActive(false);
	std::shared_ptr<ButtonNineSlice> load = ButtonNineSlice::Create(This(), view.getCenter() + sf::Vector2f(2.5f, 85.f),
		"load", L"불러오기", std::bind(&SceneMenu::Load, this));


	load->SetSize({ 450.f,250.f });

}

void SceneMenu::Draw(sf::RenderWindow& window)
{
	if (doDrawBackground)
	{
		const sf::View& preView = window.getView();
		window.setView(view);
		window.draw(background);
		window.setView(preView);
	}
	Scene::Draw(window);
}

void SceneMenu::Continue()
{
	std::shared_ptr<SceneGame> sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(SceneManager::Get("SceneGame"));
	sceneGame->Reset();
	sceneGame->LoadGame();
	New();
}

void SceneMenu::GameContinue()
{
	SceneManager::Use("SceneGameUI");
	SceneManager::Wait("SceneMenu");
}

void SceneMenu::New()
{
	lastGame->SetActive(false);
	newGame->SetActive(false);
	saveGame->SetActive(true);
	playGame->SetActive(true);
	resetGame->SetActive(true);
	SceneManager::Use("SceneGame");
	SceneManager::Use("SceneGameUI");
	SceneManager::Wait("SceneMenu");
	SceneManager::Unuse("SceneTitle");
	menuBgm.stop();
}

void SceneMenu::Option()
{
}

void SceneMenu::Save()
{
	std::shared_ptr<SceneGame> sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(SceneManager::Get("SceneGame"));
	sceneGame->SaveGame();
}

void SceneMenu::Load()
{
	std::shared_ptr<SceneGame> sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(SceneManager::Get("SceneGame"));
	sceneGame->Reset();
	sceneGame->LoadGame();
}

void SceneMenu::GameReset()
{
	std::dynamic_pointer_cast<SceneGame, Scene>(SceneManager::Get("SceneGame"))->Reset();
	std::dynamic_pointer_cast<SceneGameUI, Scene>(SceneManager::Get("SceneGameUI"))->Reset();
}

void SceneMenu::UseBackground()
{
	doDrawBackground = true;
}
