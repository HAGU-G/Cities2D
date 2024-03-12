#include "pch.h"
#include "SceneMenu.h"
#include <ObjectButton.h>
#include <ButtonNineSlice.h>
#include <SceneGame.h>
#include <SceneGameUI.h>
#include "DataManager.h"

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
		"continue", L"이어하기", std::bind(&SceneMenu::Continue, this));
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

	//불러오기
	loadView.setSize(450.f,250.f);
	loadView.setCenter(view.getCenter().x + 2.5f+225.f, view.getCenter().y + 85.f + 125.f);
	loadView.setViewport({ (view.getCenter().x + 2.5f) / resetView.getSize().x,(view.getCenter().y + 85.f) / resetView.getSize().y
		,450.f / resetView.getSize().x, 250.f / resetView.getSize().y });

	load = std::make_shared<ButtonNineSlice>(This(), sf::Vector2f(view.getCenter().x+ 2.5f, view.getCenter().y + 85.f),
		"load", L"불러오기", std::bind(&SceneMenu::Load, this));
	load->Init();
	load->Reset();

}

void SceneMenu::PreUpdate(float timeDelta, float timeScale)
{
	Scene::PreUpdate(timeDelta, timeScale);
	load->PreUpdate(timeDelta, timeScale);
}

void SceneMenu::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);
	load->Update(timeDelta, timeScale);
}

void SceneMenu::PostUpdate(float timeDelta, float timeScale)
{
	Scene::PostUpdate(timeDelta, timeScale);
	load->PostUpdate(timeDelta, timeScale);
}

void SceneMenu::Draw(sf::RenderWindow& window)
{
	const sf::View& preView = window.getView();
	if (doDrawBackground)
	{
		window.setView(view);
		window.draw(background);
	}
	Scene::Draw(window);
	window.setView(loadView);
	load->Draw(window);
	window.setView(preView);
}

void SceneMenu::Reset()
{
	Scene::Reset();
	DataManager::LoadConfig();
}

void SceneMenu::Continue()
{
	std::shared_ptr<SceneGame> sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(SceneManager::Get("SceneGame"));
	sceneGame->Reset();
	sceneGame->SetMayorName(GameManager::lastGameName);
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
	DataManager::SaveConfig();
}

void SceneMenu::Load()
{
	std::shared_ptr<SceneGame> sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(SceneManager::Get("SceneGame"));
	sceneGame->Reset();
	sceneGame->LoadGame();
	DataManager::SaveConfig();
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
