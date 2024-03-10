#include "pch.h"
#include "SceneGameUI.h"
#include "SceneGame.h"
#include "SceneMenu.h"
#include <ObjectButton.h>
#include <ButtonNameTag.h>

SceneGameUI::SceneGameUI(const std::string& name)
	:Scene(name)
{
}

void SceneGameUI::AddResource()
{
	SFGM_TEXTURE.Add("resource/ui/optionSquare.png");
	SFGM_TEXTURE.Add("resource/ui/optionSquareOn.png");
	SFGM_TEXTURE.Add("resource/ui/optionSquareDown.png");
	SFGM_TEXTURE.Add("resource/ui/r_.png");
	SFGM_TEXTURE.Add("resource/ui/r_On.png");
	SFGM_TEXTURE.Add("resource/ui/r_Down.png");
	SFGM_TEXTURE.Add("resource/ui/c_.png");
	SFGM_TEXTURE.Add("resource/ui/c_On.png");
	SFGM_TEXTURE.Add("resource/ui/c_Down.png");
	SFGM_TEXTURE.Add("resource/ui/i_.png");
	SFGM_TEXTURE.Add("resource/ui/i_On.png");
	SFGM_TEXTURE.Add("resource/ui/i_Down.png");
	SFGM_TEXTURE.Add("resource/ui/destroy.png");
	SFGM_TEXTURE.Add("resource/ui/destroyOn.png");
	SFGM_TEXTURE.Add("resource/ui/destroyDown.png");
	SFGM_TEXTURE.Add("resource/ui/road.png");
	SFGM_TEXTURE.Add("resource/ui/roadOn.png");
	SFGM_TEXTURE.Add("resource/ui/roadDown.png");
	SFGM_TEXTURE.Add("resource/ui/play.png");
	SFGM_TEXTURE.Add("resource/ui/playOn.png");
	SFGM_TEXTURE.Add("resource/ui/playDown.png");
	SFGM_TEXTURE.Add("resource/ui/pause.png");
	SFGM_TEXTURE.Add("resource/ui/pauseOn.png");
	SFGM_TEXTURE.Add("resource/ui/pauseDown.png");
	SFGM_TEXTURE.Add("resource/ui/rci.png");
	SFGM_TEXTURE.Add("resource/ui/rciOn.png");
	SFGM_TEXTURE.Add("resource/ui/rciDown.png");
	SFGM_TEXTURE.Add("resource/ui/citizen.png");
	SFGM_TEXTURE.Add("resource/ui/citizenOn.png");
	SFGM_TEXTURE.Add("resource/ui/citizenDown.png");
	SFGM_TEXTURE.Add("resource/ui/grid.png");
	SFGM_TEXTURE.Add("resource/ui/gridOn.png");
	SFGM_TEXTURE.Add("resource/ui/gridDown.png");
	SFGM_TEXTURE.Add("resource/ui/money.png");
	SFGM_TEXTURE.Add("resource/ui/moneyOn.png");
	SFGM_TEXTURE.Add("resource/ui/moneyDown.png");

	SFGM_FONT.Add("resource/font/ROKAF Sans Medium.ttf");

	SFGM_SOUNDBUFFER.Add("resource/sfx/InMouse.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/Click.wav");
}

void SceneGameUI::Init()
{
	sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(SceneManager::Get("SceneGame"));
	Scene::Init();

	//상단
	ObjectButton::Create(This(), { view.getSize().x - 5.f, 5.f }, "optionSquare", std::bind(&SceneGameUI::Menu, this))->SetOrigin(ORIGIN::TR);

	//하단
	underBarBack.setSize({ view.getSize().x, 140.f });
	underBarFront.setSize({ view.getSize().x, 70.f });
	underBarBack.setPosition({ 0.f,view.getSize().y });
	underBarFront.setPosition({ 0.f,view.getSize().y });
	underBarBack.setFillColor({ 40,40,60,180 });
	underBarFront.setFillColor({ 40,40,60,180 });
	tool::SetOrigin(underBarBack, ORIGIN::LB);
	tool::SetOrigin(underBarFront, ORIGIN::LB);

	//2단
	float secondFloarY = view.getSize().y - 105.f;
	buttonRCI = ButtonNameTag::Create(This(), { 5.f, secondFloarY }, "rci", "rci");
	buttonRCI->SetOrigin(ORIGIN::LM);
	buttonRCI->SetWidth(200);
	buttonRoad = ObjectButton::Create(This(), { view.getCenter().x - 71.5f, secondFloarY }, "road", std::bind(&SceneGameUI::Road, this));
	buttonRoad->SetOrigin(ORIGIN::RM);
	buttonR = ObjectButton::Create(This(), { view.getCenter().x - 2.5f, secondFloarY }, "r_", std::bind(&SceneGameUI::R, this));
	buttonR->SetOrigin(ORIGIN::RM);
	buttonC = ObjectButton::Create(This(), { view.getCenter().x + 2.5f, secondFloarY }, "c_", std::bind(&SceneGameUI::C, this));
	buttonC->SetOrigin(ORIGIN::LM);
	buttonI = ObjectButton::Create(This(), { view.getCenter().x + 71.5f, secondFloarY }, "i_", std::bind(&SceneGameUI::I, this));
	buttonI->SetOrigin(ORIGIN::LM);
	buttonDestroy = ObjectButton::Create(This(), { view.getSize().x - 5.f, secondFloarY }, "destroy", std::bind(&SceneGameUI::Destroy, this));
	buttonDestroy->SetOrigin(ORIGIN::RM);

	//1단
	float firstFloarY = view.getSize().y - 35.f;
	buttonPlay = ObjectButton::Create(This(), { 5.f, firstFloarY }, "play", std::bind(&SceneGameUI::Play, this));
	buttonPlay->SetOrigin(ORIGIN::LM);
	buttonPlay->OnlyDown();
	buttonPause = ObjectButton::Create(This(), { 74.f, firstFloarY }, "pause", std::bind(&SceneGameUI::Pause, this));
	buttonPause->SetOrigin(ORIGIN::LM);
	buttonPause->OnlyDown();
	buttonGrid = ButtonNameTag::Create(This(), { view.getSize().x - 685.f, firstFloarY }, "grid", "grid");
	buttonGrid->SetOrigin(ORIGIN::RM);
	buttonGrid->SetWidth(300);
	buttonCitizen = ButtonNameTag::Create(This(), { view.getSize().x - 345.f, firstFloarY }, "citizen", L"시민");
	buttonCitizen->SetOrigin(ORIGIN::RM);
	buttonCitizen->SetWidth(300);
	buttonMoney = ButtonNameTag::Create(This(), { view.getSize().x - 5.f, firstFloarY }, "money", "money");
	buttonMoney->SetOrigin(ORIGIN::RM);
	buttonMoney->SetWidth(300);



	buttonPlay->Select();
	Play();

	tempText.setFillColor(sf::Color::White);
	tempText.setCharacterSize(40);
	tempText.setFont(SFGM_FONT.Get(""));


}

void SceneGameUI::Draw(sf::RenderWindow& window)
{
	const sf::View& preView = window.getView();
	window.setView(view);
	window.draw(underBarBack);
	window.draw(underBarFront);
	window.draw(tempText);
	window.setView(preView);

	Scene::Draw(window);

}
void SceneGameUI::PreUpdate(float timeDelta, float timeScale)
{
	std::shared_ptr<SceneGame> sceneGame = this->sceneGame.lock();

	if (IOManager::IsKeyPress(sf::Mouse::Left) && !underBarBack.getGlobalBounds().contains(GetMousePosWorld()))
	{
		switch (clickMode)
		{
		case -1:
			sceneGame->DeleteObjectTile(sceneGame->GetMousePosGrid());
			break;
		case 0:
			break;
		case 1:
			sceneGame->CreateObjectTile(type, sceneGame->GetMousePosGrid());

			break;
		}
	}

	if (IOManager::IsKeyDown(sf::Mouse::Right))
	{
		switch (clickMode)
		{
		case -1:
			UnSeleteAll();
			break;
		case 0:
			break;
		case 1:
			UnSeleteAll();
			break;
		}
	}

	Scene::PreUpdate(timeDelta, timeScale);
}
void SceneGameUI::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);
}

void SceneGameUI::Menu()
{
	buttonPause->Select();
	Pause();
	SceneManager::Wait("SceneGameUI");
	SceneManager::Use("SceneMenu");
	std::dynamic_pointer_cast<SceneMenu, Scene>(SceneManager::Get("SceneMenu"))->UseBackground();
}

void SceneGameUI::UnSeleteAll()
{
	buttonRoad->UnSelect();
	buttonR->UnSelect();
	buttonC->UnSelect();
	buttonI->UnSelect();
	buttonDestroy->UnSelect();
	clickMode = 0;
	rci = RCI();
	type = GAME_OBJECT_TYPE::NONE;
}

void SceneGameUI::UpdateRCIGraph(int r, int c, int i)
{
	tempText.setString(std::to_string(r) + " " + std::to_string(c) + " " + std::to_string(i));
}

void SceneGameUI::SetTempText(const std::string& str)
{
	tempText.setString(str);
}


void SceneGameUI::Play()
{
	buttonPause->UnSelect();
	sceneGame.lock()->SetTimeScale(1.f);
}
void SceneGameUI::Pause()
{
	buttonPlay->UnSelect();
	sceneGame.lock()->SetTimeScale(0.f);
}

void SceneGameUI::Road()
{
	buttonR->UnSelect();
	buttonC->UnSelect();
	buttonI->UnSelect();
	buttonDestroy->UnSelect();

	if (!buttonRoad->IsSelete())
	{
		clickMode = 0;
		type = GAME_OBJECT_TYPE::NONE;
	}
	else
	{
		clickMode = 1;

		rci = RCI();
		type = GAME_OBJECT_TYPE::ROAD;
	}
}
void SceneGameUI::R()
{
	buttonRoad->UnSelect();
	buttonC->UnSelect();
	buttonI->UnSelect();
	buttonDestroy->UnSelect();



	if (!buttonR->IsSelete())
	{
		clickMode = 0;
		rci = RCI();
		type = GAME_OBJECT_TYPE::NONE;
	}
	else
	{
		clickMode = 1;

		rci.residence = 10;
		rci.commerce = 0;
		rci.industry = 0;
		type = GAME_OBJECT_TYPE::HOME;
	}
}
void SceneGameUI::C()
{
	buttonRoad->UnSelect();
	buttonR->UnSelect();
	buttonI->UnSelect();
	buttonDestroy->UnSelect();

	rci.residence = 0;
	rci.commerce = 1;
	rci.industry = 0;

	if (!buttonC->IsSelete())
	{
		clickMode = 0;
		rci = RCI();
		type = GAME_OBJECT_TYPE::NONE;
	}
	else
	{
		clickMode = 1;

		rci.residence = 0;
		rci.commerce = 10;
		rci.industry = 0;
		type = GAME_OBJECT_TYPE::SHOP;
	}
}
void SceneGameUI::I()
{
	buttonRoad->UnSelect();
	buttonR->UnSelect();
	buttonC->UnSelect();
	buttonDestroy->UnSelect();

	rci.residence = 0;
	rci.commerce = 0;
	rci.industry = 30;

	if (!buttonI->IsSelete())
	{
		clickMode = 0;
		rci = RCI();
		type = GAME_OBJECT_TYPE::NONE;
	}
	else
	{
		clickMode = 1;

		rci.residence = 0;
		rci.commerce = 0;
		rci.industry = 10;
		type = GAME_OBJECT_TYPE::WORK_PLACE;
	}
}
void SceneGameUI::Destroy()
{
	buttonRoad->UnSelect();
	buttonR->UnSelect();
	buttonC->UnSelect();
	buttonI->UnSelect();


	if (!buttonDestroy->IsSelete())
	{
		clickMode = 0;
	}
	else
	{
		clickMode = -1;

		rci = RCI();
		type = GAME_OBJECT_TYPE::NONE;
	}
}