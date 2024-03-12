#include "pch.h"
#include "SceneGame.h"

#include <ctime>
#include "CsvFile.h"

#include "_Include_Tile.h"
#include "ObjectIndicater.h"
#include "ObjectUnit.h"
#include "DataManager.h"
#include "SceneGameUI.h"

SceneGame::SceneGame(const std::string& name)
	:Scene(name)
{
}

void SceneGame::AddResource()
{
	//Texture
	SFGM_TEXTURE.Add("resource/tile/Buildings.png");
	SFGM_TEXTURE.Add("resource/tile/Tile.png");
	SFGM_TEXTURE.Add("resource/unit/Cat-1-Run.png");
	SFGM_TEXTURE.Add("resource/unit/Cat-1-Walk.png");
	SFGM_TEXTURE.Add("resource/unit/Cat-2-Run.png");
	SFGM_TEXTURE.Add("resource/unit/Cat-2-Walk.png");
	SFGM_TEXTURE.Add("resource/unit/Cat-5-Run.png");
	SFGM_TEXTURE.Add("resource/unit/Cat-5-Walk.png");
	SFGM_TEXTURE.Add("resource/unit/Cat-6-Run.png");
	SFGM_TEXTURE.Add("resource/unit/Cat-6-Walk.png");

	//Music
	SFGM_SOUNDBUFFER.Add("resource/music/LittleClose.wav");
	SFGM_SOUNDBUFFER.Add("resource/music/LittleFar.wav");
	SFGM_SOUNDBUFFER.Add("resource/music/MiddleClose.wav");
	SFGM_SOUNDBUFFER.Add("resource/music/MiddleFar.wav");
	SFGM_SOUNDBUFFER.Add("resource/music/NightClose.wav");
	SFGM_SOUNDBUFFER.Add("resource/music/NightFar.wav");

	//sfx
	SFGM_SOUNDBUFFER.Add("resource/sfx/build.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/cat_purring.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/destroy.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/Konbini.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/sparrows.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/type_in.wav");


}

void SceneGame::Init()
{
	useGlobalTimeScale = false;

	//선택된 타일 표시용 (임시)
	Scene::Init();
	//초기 카메라 위치 -> TODO 게임 저장시 저장하여 다시 불러올 수 있도록

	background.setFillColor({ 20,120,20,255 });
	background.setSize(view.getSize());
	tool::SetOrigin(background, ORIGIN::M);

	groundTileMap = ObjectTileMap::Create(This());

	zoomY = view.getSize().y;
}

void SceneGame::PreUpdate(float timeDelta, float timeScale)
{
	Scene::PreUpdate(timeDelta, timeScale);
	SetMousePosGrid();
	if (gameOver)
	{

	}
	else {
		UpdateCityTime(timeDelta, timeScale);
		if (city.money < 0)
			GameOver();
	}





	citizenTimer += timeDelta * timeScale;
	if (SFGM_RCI.Instance().LeftRegidence() > 0 && citizenTimer >= citizenInterval)
	{
		citizenTimer = 0.f;
		SceneGame::AddUnit(ObjectUnit::Create(This()));
	}

	//TESTCODE 이 밑으로 전부 테스트 코드

	if (IOManager::IsKeyPress(sf::Keyboard::Q))
	{
		view.rotate(45.f * timeDelta);
		city.money -= 3;
	}
	if (IOManager::IsKeyPress(sf::Keyboard::E))
	{
		view.rotate(-45.f * timeDelta);
	}
	if (IOManager::IsKeyPress(sf::Keyboard::W))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(0.f, -200.f * timeDelta));
	}
	if (IOManager::IsKeyPress(sf::Keyboard::S))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(0.f, 200.f * timeDelta));
	}
	if (IOManager::IsKeyPress(sf::Keyboard::A))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(-200.f * timeDelta, 0.f));
	}
	if (IOManager::IsKeyPress(sf::Keyboard::D))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(200.f * timeDelta, 0.f));
	}


	if (IOManager::GetWheelDelta() > 0)
	{
		view.zoom(1.f / 1.05f);
		zoomY *= 1.f / 1.05f;
	}
	else if (IOManager::GetWheelDelta() < 0)
	{
		view.zoom(1.05f);
		zoomY *= 1.05f;
	}
	if (IOManager::IsKeyPress(sf::Mouse::Middle))
	{
		if (!isTilt)
		{
			isTilt = true;
			startTilt = GameManager::GetMousePosWindow().y;
		}
		else
		{
			tilt += (startTilt - GameManager::GetMousePosWindow().y) * 0.005;
			tilt = std::max(1.f, tilt);
			view.setSize(view.getSize().x, zoomY * tilt);
			startTilt = GameManager::GetMousePosWindow().y;
		}
	}
	if (IOManager::IsKeyUp(sf::Mouse::Middle))
	{
		isTilt = false;
	}
	if (IOManager::IsKeyDown(sf::Keyboard::Num0))
	{
		view = resetView;
		tilt = 1.f;
	}

	background.setPosition(view.getCenter());
}

void SceneGame::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);
}

void SceneGame::PostUpdate(float timeDelta, float timeScale)
{
	while (!deleteTileDeque.empty())
	{
		sf::Vector2i gridCoord = deleteTileDeque.front();
		deleteTileDeque.pop_front();

		if (gridInfo[gridCoord.x][gridCoord.y].second.expired())
			continue;

		auto& adjacent = gridInfo[gridCoord.x][gridCoord.y].second.lock()->GetAdjacent();
		DeleteObject(gridInfo[gridCoord.x][gridCoord.y].second.lock()->GetKey());
		gridInfo[gridCoord.x].erase(gridCoord.y);

		if (!adjacent.empty())
		{
			for (auto& pair : adjacent)
			{
				if (!pair.second.expired())
					pair.second.lock()->RemoveAdjacent(ObjectTile::ADDIREC(~pair.first));
			}
			adjacent.clear();
		}

		groundTileMap->UpdateTile(gridCoord);
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(0, -1));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(0, 1));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(1, 0));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(-1, 0));

		if (unitOnGrid[gridCoord.x][gridCoord.y].size() > 0)
		{
			sf::Vector2i teleport = ObjectTile::FindShortPath(gridCoord, gridInfo);
			if (teleport != gridCoord)
			{
				for (auto& ptr : unitOnGrid[gridCoord.x][gridCoord.y])
				{
					if (!ptr.expired())
					{
						std::shared_ptr<ObjectUnit> tempPtr = ptr.lock();
						if (tempPtr->GetNextTile().expired()||tempPtr->GetNextTile().lock()->GetGridCoord() == gridCoord)
							tempPtr->SetPosition({ sf::Vector2f(teleport).x * gridSize.x + gridSize.x * 0.5f,
								sf::Vector2f(teleport).y * gridSize.y + gridSize.y * 0.5f });
						else
							tempPtr->SetPosition(tempPtr->GetNextTile().lock()->GetGridCenterPos());
					}
				}
			}
		}
	}

	Scene::PostUpdate(timeDelta, timeScale);
	city.doPayTex = false;
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	const sf::View& preView = window.getView();
	window.setView(view);
	//window.draw(background);
	window.setView(preView);
	Scene::Draw(window);
}

void SceneGame::Reset()
{
	gameOver = false;
	gridInfo.clear();
	unitOnGrid.clear();

	gameObjectList.clear();
	drawList.clear();

	AddObject(groundTileMap);
	Scene::Reset();

	AddObject(std::make_shared<ObjectIndicater>(This(), GAME_OBJECT_TYPE::NONE))->Init();


	city = CITY();

	time_t cT_t = time(NULL);
	tm cT;
	localtime_s(&cT, &cT_t);
	cT.tm_hour = 6;
	city.cityTime = mktime(&cT);
	city.lastMonth = cT.tm_mon;
	city.lastDay = cT.tm_mday;

	SFGM_RCI.Reset();

}

void SceneGame::Release()
{
	groundTileMap.reset();
	unitList.clear();
	unitOnGrid.clear();
	Scene::Release();
}

std::shared_ptr<ObjectUnit> SceneGame::AddUnit(const std::shared_ptr<ObjectUnit>& unit)
{
	unitList.insert(std::make_pair(unit->GetKey(), unit));
	return unit;
}

void SceneGame::UpdateCityTime(float timeDelta, float TimeScale)
{
	city.cityTimer += timeDelta * TimeScale;
	if (city.cityTimer > city.cityInterval)
	{
		city.cityTimer = 0.f;
		city.cityTime += 3600;

		tm tex;
		localtime_s(&tex, &city.cityTime);
		if (city.lastDay != tex.tm_mday)
		{
			city.lastDay = tex.tm_mday;
			MoneyReport();
		}
		if (city.lastMonth != tex.tm_mon)
		{
			city.lastMonth = tex.tm_mon;
			city.doPayTex = true;
			city.money += city.moneyTex;
			city.moneyTex = 0;
		}
		std::dynamic_pointer_cast<SceneGameUI, Scene>(SceneManager::Get("SceneGameUI"))->SetCityTimeString(city.cityTime);
	}
}

void SceneGame::MoneyProfit(unsigned int value)
{
	city.moneyProfit += value;
}

void SceneGame::MoneyTex(int value)
{
	city.moneyTex += value;
}

void SceneGame::MoneyReport()
{
	city.money += city.moneyProfit;
	city.moneyProfit = 0;
}

bool SceneGame::CreateObjectTile(RCI rci, const sf::Vector2i& gridCoord, GAME_OBJECT_TYPE type)
{
	if (gridInfo[gridCoord.x][gridCoord.y].first == GAME_OBJECT_TYPE::NONE)
	{
		gridInfo[gridCoord.x][gridCoord.y].first = type;
		if (type == GAME_OBJECT_TYPE::ROAD)
		{
			gridInfo[gridCoord.x][gridCoord.y].second = TileRoad::Create(This(), gridCoord);
		}
		else if (type >= GAME_OBJECT_TYPE::BUILDING && type < GAME_OBJECT_TYPE::BUILDING_END)
		{
			gridInfo[gridCoord.x][gridCoord.y].second = TileBuilding::Create(rci, This(), gridCoord, type);
		}
		else
		{
			return false;
		}

		groundTileMap->UpdateTile(gridCoord);
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(0, -1));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(0, 1));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(1, 0));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(-1, 0));
		return true;
	}

	return false;

}

void SceneGame::OrganizeGridInfo()
{
	for (auto& x : gridInfo)
	{
		auto it = x.second.begin();
		while (it != x.second.end())
		{
			if (it->second.second.expired())
				it = x.second.erase(it);
			else
				it++;
		}
	}
	for (auto& x : unitOnGrid)
	{
		for (auto& y : x.second)
		{
			auto it = y.second.begin();
			while (it != y.second.end())
			{
				if (it->expired())
					it = y.second.erase(it);
				else
					it++;
			}
		}
	}
}

void SceneGame::SetMayorName(const std::string& str)
{
	city.mayorName = str;
}

bool SceneGame::MoneyUse(unsigned int value)
{
	if (city.money < value)
		return false;

	city.money -= value;
	return true;
}

void SceneGame::DeleteObjectTile(const sf::Vector2i& gridCoord)
{
	deleteTileDeque.push_back(gridCoord);
}

const GridInfo& SceneGame::GetGridInfo()
{
	OrganizeGridInfo();
	return gridInfo;
}

GridInfo& SceneGame::GetGridInfoRaw()
{
	OrganizeGridInfo();
	return gridInfo;
}

void SceneGame::SaveGame()
{
	DataManager::SaveMayor(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
	DataManager::SaveTile(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
	DataManager::SaveUnit(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
}

void SceneGame::LoadGame()
{
	DataManager::LoadMayor(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
	DataManager::LoadTile(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
	DataManager::LoadUnit(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
	std::dynamic_pointer_cast<SceneGameUI, Scene>(SceneManager::Get("SceneGameUI"))->Reset();
}

void SceneGame::LoadMayor(CITY city)
{
	this->city = city;
}

bool SceneGame::LoadObjectTile(const RCI& rci, const sf::Vector2i& gridCoord,
	const std::list<GAME_OBJECT_TAG>& tagList, const sf::IntRect& rect, GAME_OBJECT_TYPE type)
{
	if (gridInfo[gridCoord.x][gridCoord.y].first == GAME_OBJECT_TYPE::NONE)
	{
		gridInfo[gridCoord.x][gridCoord.y].first = type;
		if (type == GAME_OBJECT_TYPE::ROAD)
		{
			gridInfo[gridCoord.x][gridCoord.y].second = TileRoad::Create(This(), gridCoord, tagList, rect);
		}
		else if (type >= GAME_OBJECT_TYPE::BUILDING && type < GAME_OBJECT_TYPE::BUILDING_END)
		{
			gridInfo[gridCoord.x][gridCoord.y].second = TileBuilding::Create(rci, This(), gridCoord, tagList, rect, type);
		}
		else
		{
			return false;
		}

		groundTileMap->UpdateTile(gridCoord);
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(0, -1));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(0, 1));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(1, 0));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(-1, 0));
		return true;
	}

	return false;
}

void SceneGame::GameOver()
{
	gameOver = true;
}

void SceneGame::SetMousePosGrid()
{
	if (mousePosWorld.x >= 0)
		mousePosGrid.x = mousePosWorld.x / gridSize.x;
	else
		mousePosGrid.x = floor(mousePosWorld.x / gridSize.x);

	if (mousePosWorld.y >= 0)
		mousePosGrid.y = mousePosWorld.y / gridSize.y;
	else
		mousePosGrid.y = floor(mousePosWorld.y / gridSize.y);
}