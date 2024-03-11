#include "pch.h"
#include "SceneGameUI.h"
#include "SceneGame.h"
#include "SceneMenu.h"
#include "ObjectButton.h"
#include "ButtonNameTag.h"
#include "ObjectUnit.h"
#include "ObjectTile.h"
#include "ObjectTileMap.h"

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
	SFGM_TEXTURE.Add("resource/ui/fast.png");
	SFGM_TEXTURE.Add("resource/ui/fastOn.png");
	SFGM_TEXTURE.Add("resource/ui/fastDown.png");

	SFGM_FONT.Add("resource/font/ROKAF Sans Medium.ttf");

	SFGM_SOUNDBUFFER.Add("resource/sfx/InMouse.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/Click.wav");
}

void SceneGameUI::Init()
{
	sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(SceneManager::Get("SceneGame"));
	Scene::Init();

	//상단
	buttonMenu = ObjectButton::Create(This(), { view.getSize().x - 5.f, 5.f }, "optionSquare", std::bind(&SceneGameUI::Menu, this), false);
	buttonMenu->SetOrigin(ORIGIN::TR);
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
	buttonRCI = ButtonNameTag::Create(This(), { 5.f, secondFloarY }, "rci");
	buttonRCI->SetOrigin(ORIGIN::LM);
	buttonRCI->SetWidth(330);
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

	rBar.setFillColor(sf::Color::Green);
	rBar.setSize({ buttonRCI->GetBound().width - 90.f, 20.f });
	rBar.setOrigin(0.f, 30.f);
	rBar.setPosition(buttonRCI->GetPosition() + sf::Vector2f(65.f, 0.f));

	cBar.setFillColor(sf::Color::Blue);
	cBar.setSize({ buttonRCI->GetBound().width - 90.f, 20.f });
	cBar.setOrigin(0.f, 10.f);
	cBar.setPosition(buttonRCI->GetPosition() + sf::Vector2f(65.f, 0.f));

	iBar.setFillColor(sf::Color::Yellow);
	iBar.setSize({ buttonRCI->GetBound().width - 90.f, 20.f });
	iBar.setOrigin(0.f, -10.f);
	iBar.setPosition(buttonRCI->GetPosition() + sf::Vector2f(65.f, 0.f));

	//1단
	float firstFloarY = view.getSize().y - 35.f;
	buttonPause = ObjectButton::Create(This(), { 5.f, firstFloarY }, "pause", std::bind(&SceneGameUI::Pause, this));
	buttonPause->SetOrigin(ORIGIN::LM);
	buttonPause->OnlyDown();
	buttonPlay = ObjectButton::Create(This(), { 74.f, firstFloarY }, "play", std::bind(&SceneGameUI::Play, this));
	buttonPlay->SetOrigin(ORIGIN::LM);
	buttonPlay->OnlyDown();
	button4x = ObjectButton::Create(This(), { 141.f, firstFloarY }, "fast", std::bind(&SceneGameUI::Fast, this));
	button4x->SetOrigin(ORIGIN::LM);
	button4x->OnlyDown();
	buttonCityTime = ButtonNameTag::Create(This(), { 206.f, firstFloarY }, "grid", "0");
	buttonCityTime->SetOrigin(ORIGIN::LM);
	buttonCityTime->SetWidth(400);

	buttonGrid = ButtonNameTag::Create(This(), { view.getSize().x - 685.f, firstFloarY }, "grid", "0");
	buttonGrid->SetOrigin(ORIGIN::RM);
	buttonGrid->SetWidth(300);
	buttonGrid->SetFuntion(std::bind(&ObjectTileMap::TurnDrawLine, sceneGame.lock()->GetTileMap().lock().get()));
	buttonCitizen = ButtonNameTag::Create(This(), { view.getSize().x - 345.f, firstFloarY }, "citizen", "0");
	buttonCitizen->SetOrigin(ORIGIN::RM);
	buttonCitizen->SetWidth(300);
	buttonMoney = ButtonNameTag::Create(This(), { view.getSize().x - 5.f, firstFloarY }, "money", "0");
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


	Scene::Draw(window);

	window.draw(rBar);
	window.draw(cBar);
	window.draw(iBar);

	window.setView(preView);

}
void SceneGameUI::Reset()
{
	Scene::Reset();
	SetCityTimeString(sceneGame.lock()->GetCityTime());
	
}
void SceneGameUI::PreUpdate(float timeDelta, float timeScale)
{
	std::shared_ptr<SceneGame> sceneGame = this->sceneGame.lock();


	if (IOManager::IsKeyPress(sf::Mouse::Left) && !underBarBack.getGlobalBounds().contains(GetMousePosWorld())
		&& !buttonMenu->GetBound().contains(GetMousePosWorld()))
	{

		switch (clickMode)
		{
		case -1:
			sceneGame->DeleteObjectTile(sceneGame->GetMouseGridCoord());
			break;
		case 0:
			break;
		case 1:
			if (sceneGame->GetTileInfo(sceneGame->GetMouseGridCoord()).first == GAME_OBJECT_TYPE::NONE)
			{
				if (sceneGame->MoneyUse(rci.cost))
					sceneGame->CreateObjectTile(rci, sceneGame->GetMouseGridCoord(), type);
			}
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
	buttonCitizen->SetString(to_string(ObjectUnit::GetUnitCount()));
	buttonGrid->SetString(to_string(ObjectTile::GetTileCount()));

	float max = std::max(GM_RCI.NeedRegidence() + GM_RCI.NeedCommerce() + GM_RCI.NeedIndustry(), 10);
	rBar.setScale(GM_RCI.NeedRegidence() / max, 1.f);
	cBar.setScale(GM_RCI.NeedCommerce() / max, 1.f);
	iBar.setScale(GM_RCI.NeedIndustry() / max, 1.f);

	buttonMoney->SetString(tool::ThousandsSeparator(sceneGame.lock()->GetMoney()));

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

void SceneGameUI::SetCityTimeString(const time_t& cityTime)
{
	tm cT;
	localtime_s(&cT, &cityTime);

	std::wstring hourZero;
	if (cT.tm_hour < 10)
		hourZero = L"0";

	buttonCityTime->SetString(
		std::to_wstring(cT.tm_year+1900) + L"년 "
		+ std::to_wstring(cT.tm_mon+1) + L"월 "
		+ std::to_wstring(cT.tm_mday) + L"일 "
		+ hourZero + std::to_wstring(cT.tm_hour) + L"시");
}



void SceneGameUI::Pause()
{
	buttonPlay->UnSelect();
	button4x->UnSelect();
	sceneGame.lock()->SetTimeScale(0.f);
}
void SceneGameUI::Play()
{
	buttonPause->UnSelect();
	button4x->UnSelect();
	sceneGame.lock()->SetTimeScale(1.f);
}
void SceneGameUI::Fast()
{
	buttonPause->UnSelect();
	buttonPlay->UnSelect();
	sceneGame.lock()->SetTimeScale(4.f);
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
		rci.cost = 10;
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
		type = GAME_OBJECT_TYPE::NONE;
	}
	else
	{
		clickMode = 1;

		rci = RCI();
		rci.cost = 10;
		rci.tex = 10;
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

	if (!buttonC->IsSelete())
	{
		clickMode = 0;
		type = GAME_OBJECT_TYPE::NONE;
	}
	else
	{
		clickMode = 1;

		rci = RCI();
		rci.cost = 20;
		rci.tex = 20;
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

	if (!buttonI->IsSelete())
	{
		clickMode = 0;
		rci = RCI();
		type = GAME_OBJECT_TYPE::NONE;
	}
	else
	{
		clickMode = 1;

		rci = RCI();
		rci.cost = 30;
		rci.tex = 30;
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