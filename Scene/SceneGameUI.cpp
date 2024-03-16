#include "pch.h"
#include "SceneGameUI.h"
#include "SceneGame.h"
#include "SceneMenu.h"
#include "ObjectButton.h"
#include "ButtonNameTag.h"
#include "ObjectUnit.h"
#include "ObjectTile.h"
#include "ObjectTileMap.h"
#include "ObjectIndicater.h"

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
	SFGM_TEXTURE.Add("resource/ui/r_b_1.png");
	SFGM_TEXTURE.Add("resource/ui/r_b_1On.png");
	SFGM_TEXTURE.Add("resource/ui/r_b_1Down.png");
	SFGM_TEXTURE.Add("resource/ui/r_b_2.png");
	SFGM_TEXTURE.Add("resource/ui/r_b_2On.png");
	SFGM_TEXTURE.Add("resource/ui/r_b_2Down.png");
	SFGM_TEXTURE.Add("resource/ui/r_b_3.png");
	SFGM_TEXTURE.Add("resource/ui/r_b_3On.png");
	SFGM_TEXTURE.Add("resource/ui/r_b_3Down.png");
	SFGM_TEXTURE.Add("resource/ui/i_b_1.png");
	SFGM_TEXTURE.Add("resource/ui/i_b_1On.png");
	SFGM_TEXTURE.Add("resource/ui/i_b_1Down.png");
	SFGM_TEXTURE.Add("resource/ui/i_b_2.png");
	SFGM_TEXTURE.Add("resource/ui/i_b_2On.png");
	SFGM_TEXTURE.Add("resource/ui/i_b_2Down.png");
	SFGM_TEXTURE.Add("resource/ui/i_b_3.png");
	SFGM_TEXTURE.Add("resource/ui/i_b_3On.png");
	SFGM_TEXTURE.Add("resource/ui/i_b_3Down.png");
	SFGM_TEXTURE.Add("resource/ui/c_b_1.png");
	SFGM_TEXTURE.Add("resource/ui/c_b_1On.png");
	SFGM_TEXTURE.Add("resource/ui/c_b_1Down.png");
	SFGM_TEXTURE.Add("resource/ui/c_b_2.png");
	SFGM_TEXTURE.Add("resource/ui/c_b_2On.png");
	SFGM_TEXTURE.Add("resource/ui/c_b_2Down.png");
	SFGM_TEXTURE.Add("resource/ui/road_b_1.png");
	SFGM_TEXTURE.Add("resource/ui/road_b_1On.png");
	SFGM_TEXTURE.Add("resource/ui/road_b_1Down.png");

	SFGM_FONT.Add("resource/font/ROKAF Sans Medium.ttf");

	SFGM_SOUNDBUFFER.Add("resource/sfx/InMouse.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/Click.wav");

	SFGM_CSVFILE.Add("data/BuildingData.csv");
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
	buttonRCI = ButtonNameTag::Create(This(), { 5.f, secondFloarY }, "rci", "", std::bind(&SceneGameUI::SetMayorName, this));
	buttonRCI->SetOrigin(ORIGIN::LM);
	buttonRCI->SetWidth(500);
	buttonRCI->SetDoToggle(true);
	buttonRCI->SetIsTextField(true);

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
	buttonPause->SetOnlyDown(true);
	pauseOutline.setSize({ view.getSize().x - 10.f, view.getSize().y - 10.f });
	tool::SetOrigin(pauseOutline, ORIGIN::MC);
	pauseOutline.setOutlineColor(sf::Color::Red);
	pauseOutline.setFillColor(sf::Color::Transparent);
	pauseOutline.setOutlineThickness(10.f);
	pauseOutline.setPosition(view.getCenter());

	buttonPlay = ObjectButton::Create(This(), { 74.f, firstFloarY }, "play", std::bind(&SceneGameUI::Play, this));
	buttonPlay->SetOrigin(ORIGIN::LM);
	buttonPlay->SetOnlyDown(true);
	button4x = ObjectButton::Create(This(), { 141.f, firstFloarY }, "fast", std::bind(&SceneGameUI::Fast, this));
	button4x->SetOrigin(ORIGIN::LM);
	button4x->SetOnlyDown(true);
	buttonCityTime = ButtonNameTag::Create(This(), { 206.f, firstFloarY }, "grid", "0");
	buttonCityTime->SetOrigin(ORIGIN::LM);
	buttonCityTime->SetWidth(400);
	buttonCityTime->SetCanReact(false);

	buttonGrid = ButtonNameTag::Create(This(), { view.getSize().x - 685.f, firstFloarY }, "grid", "0");
	buttonGrid->SetOrigin(ORIGIN::RM);
	buttonGrid->SetWidth(300);
	buttonGrid->SetFuntion(std::bind(&ObjectTileMap::TurnDrawLine, sceneGame.lock()->GetTileMap().lock().get()));
	buttonCitizen = ButtonNameTag::Create(This(), { view.getSize().x - 345.f, firstFloarY }, "citizen", "0");
	buttonCitizen->SetOrigin(ORIGIN::RM);
	buttonCitizen->SetWidth(300);
	buttonCitizen->SetCanReact(false);

	buttonMoney = ButtonNameTag::Create(This(), { view.getSize().x - 5.f, firstFloarY }, "money", "0");
	buttonMoney->SetOrigin(ORIGIN::RM);
	buttonMoney->SetWidth(300);
	buttonMoney->SetCanReact(false);
	textProfit.setFillColor(sf::Color::Green);
	textProfit.setCharacterSize(18);
	textProfit.setFont(SFGM_FONT.Get("resource/font/ROKAF Sans Medium.ttf"));
	textProfit.setPosition(buttonMoney->GetPosition() + sf::Vector2f(-32.f, 0.f - textProfit.getCharacterSize()));
	textProfit.setStyle(sf::Text::Italic);
	textProfit.setOutlineColor(sf::Color::Black);
	textProfit.setOutlineThickness(0.5f);
	tool::SetOrigin(textProfit, ORIGIN::RB);
	textTex.setFillColor(sf::Color::Green);
	textTex.setCharacterSize(18);
	textTex.setFont(SFGM_FONT.Get("resource/font/ROKAF Sans Medium.ttf"));
	textTex.setPosition(buttonMoney->GetPosition() + sf::Vector2f(-32.f, textTex.getCharacterSize() / 3));
	textTex.setStyle(sf::Text::Italic);
	textTex.setOutlineColor(sf::Color::Black);
	textTex.setOutlineThickness(0.5f);
	tool::SetOrigin(textTex, ORIGIN::RT);


	buttonPlay->Select();
	Play();

	tempText.setFillColor(sf::Color::White);
	tempText.setCharacterSize(40);
	tempText.setFont(SFGM_FONT.Get("resource/font/ROKAF Sans Medium.ttf"));

	buildingDoc = SFGM_CSVFILE.Get("data/BuildingData.csv").GetDocument();
	LoadBuildingButton();
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
	window.draw(textProfit);
	window.draw(textTex);
	if (sceneGame.lock()->GetTimeScale() == 0.f)
	{
		window.draw(pauseOutline);
	}
	window.setView(preView);

}
void SceneGameUI::Reset()
{
	Scene::Reset();
	SetCityTimeString(sceneGame.lock()->GetCityTime());
	textTex.setString(to_string(0));
	textProfit.setString(to_string(0));
	SetMayorName();

}
void SceneGameUI::PreUpdate(float timeDelta, float timeScale)
{
	std::shared_ptr<SceneGame> sceneGame = this->sceneGame.lock();

	if (sceneGame->IsGameOver())
	{
		buttonPlay->SetCanReact(false);
		buttonPause->SetCanReact(false);
		button4x->SetCanReact(false);
		buttonPlay->UnSelect();
		button4x->UnSelect();
		buttonPause->Select();
	}
	else
	{
		buttonPlay->SetCanReact(true);
		buttonPause->SetCanReact(true);
		button4x->SetCanReact(true);
	}

	if (buttonRCI->IsInputMode())
	{
		sceneGame->SetCameraFixed(true);

		if (IOManager::IsKeyUp(sf::Mouse::Left) || IOManager::IsKeyUp(sf::Mouse::Right))
			IOManager::SetDoInputText(false);
	}
	else
	{
		sceneGame->SetCameraFixed(false);

		if (!sceneGame->IsGameOver() && IOManager::IsKeyPress(sf::Mouse::Left) && !underBarBack.getGlobalBounds().contains(GetMousePosWorld())
			&& !buttonMenu->GetBound().contains(GetMousePosWorld()))
		{
			if (sceneGame->GetIndicater().lock()->CanClick())
			{
				switch (clickMode)
				{
				case -1:
					sceneGame->DeleteObjectTile(sceneGame->GetMouseGridCoord());
					break;
				case 0:
					break;
				case 1:
					if (sceneGame->MoneyUse(rci.cost))
						sceneGame->CreateObjectTile(rci, sceneGame->GetMouseGridCoord(), type);
					break;
				}
			}
		}

		if (!sceneGame->IsGameOver() && IOManager::IsKeyDown(sf::Mouse::Right))
		{
			UnSeleteAll();
		}
	}



	Scene::PreUpdate(timeDelta, timeScale);
}
void SceneGameUI::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);

	if (sceneGame.lock()->IsGameOver())
		return;

	buttonCitizen->SetString(to_string(ObjectUnit::GetUnitCount()));
	buttonGrid->SetString(to_string(ObjectTile::GetTileCount()));

	float max = std::max(SFGM_RCI.NeedRegidence() + SFGM_RCI.NeedCommerce() + SFGM_RCI.NeedIndustry(), 100);
	rBar.setScale(std::max(0.f, SFGM_RCI.NeedRegidence() / max), 1.f);
	cBar.setScale(std::max(0.f, SFGM_RCI.NeedCommerce() / max), 1.f);
	iBar.setScale(std::max(0.f, SFGM_RCI.NeedIndustry() / max), 1.f);

	buttonMoney->SetString(tool::ThousandsSeparator(sceneGame.lock()->GetMoney()));
	textProfit.setString("Daily +" + tool::ThousandsSeparator(sceneGame.lock()->GetProfit()));
	tool::SetOrigin(textProfit, ORIGIN::RB);
	int tex = sceneGame.lock()->GetTex();
	tool::SetOrigin(textTex, ORIGIN::RT);
	if (tex < 0)
	{
		textTex.setFillColor(sf::Color::Red);
		textTex.setString("Monthly" + tool::ThousandsSeparator(tex));
	}
	else
	{
		textTex.setFillColor(sf::Color::Green);
		textTex.setString("Monthly +" + tool::ThousandsSeparator(tex));
	}
}

void SceneGameUI::Menu()
{
	sceneGame.lock()->SetCameraFixed(true);
	buttonPause->Select();
	Pause();
	SceneManager::Wait("SceneGameUI");
	SceneManager::Use("SceneMenu");
	std::dynamic_pointer_cast<SceneMenu, Scene>(SceneManager::Get("SceneMenu"))->UseBackground();
}

void SceneGameUI::SetMayorName()
{
	if (buttonRCI->IsInputMode())
	{
		if (buttonRCI->GetString() == "")
			buttonRCI->SetString(sceneGame.lock()->GetCityInfo().mayorName);
		else
			sceneGame.lock()->SetMayorName(buttonRCI->GetString());
	}
	else
	{
		buttonRCI->SetString(sceneGame.lock()->GetCityInfo().mayorName);
	}
}

void SceneGameUI::UnSeleteAll()
{
	buttonRoad->UnSelect();
	for (auto& pair : roadList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonR->UnSelect();
	for (auto& pair : rList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonC->UnSelect();
	for (auto& pair : cList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonI->UnSelect();
	for (auto& pair : iList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonDestroy->UnSelect();
	SetClickMode(0);
	rci = RCI();
	type = GAME_OBJECT_TYPE::NONE;
	underBarBack.setScale(1.f, 1.f);
}

void SceneGameUI::LoadBuildingButton()
{
	roadList.clear();
	rList.clear();
	cList.clear();
	iList.clear();

	int buttonRoadCount = 0;
	int buttonRCount = 0;
	int buttonCCount = 0;
	int buttonICount = 0;
	for (int i = 0; i < buildingDoc.GetRowCount(); i++)
	{
		auto rciRow = buildingDoc.GetRow<std::string>(i);
		RCI rciInfo;

		rciInfo.residence = stoi(rciRow[1]);
		rciInfo.commerce = stoi(rciRow[2]);
		rciInfo.industry = stoi(rciRow[3]);

		rciInfo.cost = stoi(rciRow[4]);
		rciInfo.tex = stoi(rciRow[5]);

		rciInfo.texturePath = rciRow[6];
		rciInfo.textureRect.left = stoi(rciRow[7]);
		rciInfo.textureRect.top = stoi(rciRow[8]);
		rciInfo.textureRect.width = stoi(rciRow[9]);
		rciInfo.textureRect.height = stoi(rciRow[10]);

		//////////길 버튼
		if (stoi(rciRow[0]) == (int)GAME_OBJECT_TYPE::ROAD - (int)GAME_OBJECT_TYPE::TILE)
		{
			std::weak_ptr<ObjectButton> button = ObjectButton::Create(This(),
				{ view.getCenter().x + (buttonRoadCount - 1) * 69.f, underBarBack.getGlobalBounds().top - 5.f }, rciRow[11]);
			button.lock()->SetFuntion(
				[button, this, row = i]()
				{
					auto thisButton = button.lock();
					for (auto& pair : std::dynamic_pointer_cast<SceneGameUI, Scene>(thisButton->GetScene().lock())->roadList)
					{
						if (pair.first->GetKey() == thisButton->GetKey())
							continue;
						pair.first->UnSelect();
					}

					if (!thisButton->IsSelect())
					{
						SetClickMode(0);
						type = GAME_OBJECT_TYPE::NONE;
					}
					else
					{
						SetClickMode(1);
						type = GAME_OBJECT_TYPE::ROAD;
						auto rRow = buildingDoc.GetRow<std::string>(row);

						for (auto& pair : roadList)
						{
							if (pair.first->GetKey() == thisButton->GetKey())
								rci = pair.second;
						}
						sceneGame.lock()->GetIndicater().lock()->SetRCI(rci);
					}
				});
			button.lock()->SetOrigin(ORIGIN::BC);
			buttonRoadCount++;
			roadList.push_back(std::make_pair(button.lock(), rciInfo));
			button.lock()->SetActive(false);
		}
		////////주거 버튼
		else if (stoi(rciRow[0]) == (int)GAME_OBJECT_TYPE::HOME - (int)GAME_OBJECT_TYPE::TILE)
		{
			std::weak_ptr<ObjectButton> button = ObjectButton::Create(This(),
				{ view.getCenter().x + buttonRCount * 69.f, underBarBack.getGlobalBounds().top - 5.f }, rciRow[11]);
			button.lock()->SetFuntion(
				[button, this, row = i]()
				{
					auto thisButton = button.lock();
					for (auto& pair : std::dynamic_pointer_cast<SceneGameUI, Scene>(thisButton->GetScene().lock())->rList)
					{
						if (pair.first->GetKey() == thisButton->GetKey())
							continue;
						pair.first->UnSelect();
					}

					if (!thisButton->IsSelect())
					{
						SetClickMode(0);
						type = GAME_OBJECT_TYPE::NONE;
					}
					else
					{
						SetClickMode(1);
						type = GAME_OBJECT_TYPE::HOME;
						auto rRow = buildingDoc.GetRow<std::string>(row);

						for (auto& pair : rList)
						{
							if (pair.first->GetKey() == thisButton->GetKey())
								rci = pair.second;
						}
						sceneGame.lock()->GetIndicater().lock()->SetRCI(rci);
					}
				});
			button.lock()->SetOrigin(ORIGIN::BC);
			buttonRCount++;
			rList.push_back(std::make_pair(button.lock(), rciInfo));
			button.lock()->SetActive(false);
		}
		////////상업 버튼
		else if (stoi(rciRow[0]) == (int)GAME_OBJECT_TYPE::SHOP - (int)GAME_OBJECT_TYPE::TILE)
		{
			std::weak_ptr<ObjectButton> button = ObjectButton::Create(This(),
				{ view.getCenter().x + (buttonCCount + 1) * 69.f, underBarBack.getGlobalBounds().top - 5.f }, rciRow[11]);
			button.lock()->SetFuntion(
				[button, this, row = i]()
				{
					auto thisButton = button.lock();
					for (auto& pair : std::dynamic_pointer_cast<SceneGameUI, Scene>(thisButton->GetScene().lock())->cList)
					{
						if (pair.first->GetKey() == thisButton->GetKey())
							continue;
						pair.first->UnSelect();
					}

					if (!thisButton->IsSelect())
					{
						SetClickMode(0);
						type = GAME_OBJECT_TYPE::NONE;
					}
					else
					{
						SetClickMode(1);
						type = GAME_OBJECT_TYPE::SHOP;
						auto rRow = buildingDoc.GetRow<std::string>(row);

						for (auto& pair : cList)
						{
							if (pair.first->GetKey() == thisButton->GetKey())
								rci = pair.second;
						}
						sceneGame.lock()->GetIndicater().lock()->SetRCI(rci);
					}
				});
			button.lock()->SetOrigin(ORIGIN::BC);
			buttonCCount++;
			cList.push_back(std::make_pair(button.lock(), rciInfo));
			button.lock()->SetActive(false);
		}
		////////산업 버튼
		else if (stoi(rciRow[0]) == (int)GAME_OBJECT_TYPE::WORK_PLACE - (int)GAME_OBJECT_TYPE::TILE)
		{
			std::weak_ptr<ObjectButton> button = ObjectButton::Create(This(),
				{ view.getCenter().x + (buttonICount + 2) * 69.f , underBarBack.getGlobalBounds().top - 5.f }, rciRow[11]);
			button.lock()->SetFuntion(
				[button, this, row = i]()
				{
					auto thisButton = button.lock();
					for (auto& pair : std::dynamic_pointer_cast<SceneGameUI, Scene>(thisButton->GetScene().lock())->iList)
					{
						if (pair.first->GetKey() == thisButton->GetKey())
							continue;
						pair.first->UnSelect();
					}

					if (!thisButton->IsSelect())
					{
						SetClickMode(0);
						type = GAME_OBJECT_TYPE::NONE;
					}
					else
					{
						SetClickMode(1);
						type = GAME_OBJECT_TYPE::WORK_PLACE;
						auto rRow = buildingDoc.GetRow<std::string>(row);

						for (auto& pair : iList)
						{
							if (pair.first->GetKey() == thisButton->GetKey())
								rci = pair.second;
						}
						sceneGame.lock()->GetIndicater().lock()->SetRCI(rci);
					}
				});
			button.lock()->SetOrigin(ORIGIN::BC);
			buttonICount++;
			iList.push_back(std::make_pair(button.lock(), rciInfo));
			button.lock()->SetActive(false);
		}
	}
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
		std::to_wstring(cT.tm_year + 1900) + L"년 "
		+ std::to_wstring(cT.tm_mon + 1) + L"월 "
		+ std::to_wstring(cT.tm_mday) + L"일 "
		+ hourZero + std::to_wstring(cT.tm_hour) + L"시");
}

void SceneGameUI::SetClickMode(int value)
{
	clickMode = value;
	sceneGame.lock()->GetIndicater().lock()->clickMode = clickMode;
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
	sceneGame.lock()->SetTimeScale(16.f);
}

void SceneGameUI::Road()
{

	buttonR->UnSelect();
	for (auto& pair : rList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonC->UnSelect();
	for (auto& pair : cList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonI->UnSelect();
	for (auto& pair : iList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonDestroy->UnSelect();

	SetClickMode(0);

	if (!buttonRoad->IsSelect())
	{
		for (auto& pair : roadList)
		{
			pair.first->UnSelect();
			pair.first->SetActive(false);
		}
		underBarBack.setScale(1.f, 1.f);
	}
	else
	{
		underBarBack.setScale(1.f, 1.5f);
		for (auto& pair : roadList)
		{
			pair.first->SetActive(true);
		}
	}
}
void SceneGameUI::R()
{
	buttonRoad->UnSelect();
	for (auto& pair : roadList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonC->UnSelect();
	for (auto& pair : cList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonI->UnSelect();
	for (auto& pair : iList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonDestroy->UnSelect();

	SetClickMode(0);

	if (!buttonR->IsSelect())
	{
		for (auto& pair : rList)
		{
			pair.first->UnSelect();
			pair.first->SetActive(false);
		}
		underBarBack.setScale(1.f, 1.f);
	}
	else
	{
		underBarBack.setScale(1.f, 1.5f);
		for (auto& pair : rList)
		{
			pair.first->SetActive(true);
		}
	}
}
void SceneGameUI::C()
{
	buttonR->UnSelect();
	for (auto& pair : rList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonRoad->UnSelect();
	for (auto& pair : roadList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonI->UnSelect();
	for (auto& pair : iList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonDestroy->UnSelect();

	SetClickMode(0);

	if (!buttonC->IsSelect())
	{
		for (auto& pair : cList)
		{
			pair.first->UnSelect();
			pair.first->SetActive(false);
		}
		underBarBack.setScale(1.f, 1.f);
	}
	else
	{
		underBarBack.setScale(1.f, 1.5f);
		for (auto& pair : cList)
		{
			pair.first->SetActive(true);
		}
	}
}
void SceneGameUI::I()
{
	buttonR->UnSelect();
	for (auto& pair : rList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonC->UnSelect();
	for (auto& pair : cList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonRoad->UnSelect();
	for (auto& pair : roadList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonDestroy->UnSelect();

	SetClickMode(0);

	if (!buttonI->IsSelect())
	{
		for (auto& pair : iList)
		{
			pair.first->UnSelect();
			pair.first->SetActive(false);
		}
		underBarBack.setScale(1.f, 1.f);
	}
	else
	{
		underBarBack.setScale(1.f, 1.5f);
		for (auto& pair : iList)
		{
			pair.first->SetActive(true);
		}
	}
}
void SceneGameUI::Destroy()
{
	buttonRoad->UnSelect();
	for (auto& pair : roadList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonR->UnSelect();
	for (auto& pair : rList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonC->UnSelect();
	for (auto& pair : cList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	buttonI->UnSelect();
	for (auto& pair : iList)
	{
		pair.first->UnSelect();
		pair.first->SetActive(false);
	}
	underBarBack.setScale(1.f, 1.f);

	if (!buttonDestroy->IsSelect())
	{
		SetClickMode(0);
	}
	else
	{
		SetClickMode(-1);

		rci = RCI();
		type = GAME_OBJECT_TYPE::NONE;
	}
}