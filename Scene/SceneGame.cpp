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
	SFGM_TEXTURE.Add("resource/unit/Cat0-Run.png");
	SFGM_TEXTURE.Add("resource/unit/Cat0-Walk.png");
	SFGM_TEXTURE.Add("resource/unit/Cat0-Laying.png");
	SFGM_TEXTURE.Add("resource/unit/Cat1-Run.png");
	SFGM_TEXTURE.Add("resource/unit/Cat1-Walk.png");
	SFGM_TEXTURE.Add("resource/unit/Cat1-Laying.png");
	SFGM_TEXTURE.Add("resource/unit/Cat2-Run.png");
	SFGM_TEXTURE.Add("resource/unit/Cat2-Walk.png");
	SFGM_TEXTURE.Add("resource/unit/Cat2-Laying.png");
	SFGM_TEXTURE.Add("resource/unit/Cat3-Run.png");
	SFGM_TEXTURE.Add("resource/unit/Cat3-Walk.png");
	SFGM_TEXTURE.Add("resource/unit/Cat3-Laying.png");
	SFGM_TEXTURE.Add("resource/ui/destroySprite.png");

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
	SFGM_SOUNDBUFFER.Add("resource/sfx/leave.wav");
	SFGM_SOUNDBUFFER.Add("resource/sfx/homeless.wav");

	//csv
	SFGM_CSVFILE.Add("data/TileData.csv");
	SFGM_CSVFILE.Add("data/BuildingData.csv");
	SFGM_CSVFILE.Add("data/UnitData.csv");
	SFGM_CSVFILE.Add("data/CitizenData.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat0-Run.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat0-Walk.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat0-Laying.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat1-Run.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat1-Walk.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat1-Laying.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat2-Run.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat2-Walk.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat2-Laying.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat3-Run.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat3-Walk.csv");
	SFGM_CSVFILE.Add("resource/unit/Cat3-Laying.csv");

	//animation
	SFGM_ANICLIP.Add("resource/unit/Cat0-Run.csv");
	SFGM_ANICLIP.Add("resource/unit/Cat0-Walk.csv");
	SFGM_ANICLIP.Add("resource/unit/Cat0-Laying.csv");
	SFGM_ANICLIP.Add("resource/unit/Cat1-Run.csv");
	SFGM_ANICLIP.Add("resource/unit/Cat1-Walk.csv");
	SFGM_ANICLIP.Add("resource/unit/Cat1-Laying.csv");
	SFGM_ANICLIP.Add("resource/unit/Cat2-Run.csv");
	SFGM_ANICLIP.Add("resource/unit/Cat2-Walk.csv");
	SFGM_ANICLIP.Add("resource/unit/Cat2-Laying.csv");
	SFGM_ANICLIP.Add("resource/unit/Cat3-Run.csv");
	SFGM_ANICLIP.Add("resource/unit/Cat3-Walk.csv");
	SFGM_ANICLIP.Add("resource/unit/Cat3-Laying.csv");

	//font
	SFGM_FONT.Add("resource/font/DOSIyagiBoldface.ttf");

}

void SceneGame::Init()
{
	useGlobalTimeScale = false;

	Scene::Init();

	groundTileMap = ObjectTileMap::Create(This());
	indicater = ObjectIndicater::Create(This());
	zoomY = view.getSize().y;

	sf::RenderWindow& window = GameManager::GetWindow();
	frustum.nearPlane = tool::To3D(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, window.getSize().y), view));
	frustum.farPlane = tool::To3D(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, 0), view));
	frustum.leftPlane = tool::To3D(window.mapPixelToCoords(sf::Vector2i(0, window.getSize().y / 2), view));
	frustum.rightPlane = tool::To3D(window.mapPixelToCoords(sf::Vector2i(window.getSize().x, window.getSize().y / 2), view));
}

void SceneGame::PreUpdate(float timeDelta, float timeScale)
{
	doDrawLayerSort = false;
	isCameraMoved = false;
	CameraInput(timeDelta);

	mousePosWorld = GameManager::GetWindow().mapPixelToCoords(GameManager::GetMousePosWindow(), view);
	SetMousePosGrid();

	if (!gameOver)
	{
		UpdateCityTime(timeDelta, timeScale);
		if (city.money < 0)
			GameOver();
	}

	citizenTimer += timeDelta * timeScale;
	if (SFGM_RCI.Instance().LeftRegidence() > 0 && citizenTimer >= citizenInterval)
	{
		citizenTimer = 0.f;
		SceneGame::AddUnit(ObjectUnit::Create(This(), GAME_OBJECT_TYPE::CITIZEN));
	}
	if (isCameraMoved)
		OnCameraMove();
	Scene::PreUpdate(timeDelta, timeScale);
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
		IOManager::PlaySfx("resource/sfx/destroy.wav", true);
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
						if (tempPtr->GetNextTile().expired() || tempPtr->GetNextTile().lock()->GetGridCoord() == gridCoord)
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
	groundTileMap->Draw(window);
	window.setView(preView);

	Scene::Draw(window);
}

void SceneGame::Reset()
{


	gridInfo.clear();
	unitOnGrid.clear();

	gameObjectList.clear();
	drawList.clear();

	AddObject(groundTileMap);
	AddObject(indicater);

	//기본 타일 생성
	if (!isLoading)
	{
		gridInfo[0][0].first = GAME_OBJECT_TYPE::ROAD;
		gridInfo[0][0].second = TileRoad::Create(This(), { 0, 0 });
		groundTileMap->UpdateTile({ 0, 0 });
		ChangeBGM();
		gameOver = false;
	}

	Scene::Reset();
	view.setCenter(gridSize * 0.5f);


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

void SceneGame::Enter()
{
	if (ObjectUnit::GetUnitCount() >= 100)
	{
		isMiddleCity = true;
		IOManager::BGMSyncPlay("resource/music/MiddleClose.wav", "resource/music/MiddleFar.wav", true, true);
	}
	else if (ObjectUnit::GetUnitCount() <= 50)
	{
		isMiddleCity = false;
		IOManager::BGMSyncPlay("resource/music/LittleClose.wav", "resource/music/LittleFar.wav", true, true);
	}
	SetBGMSync();
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
	ChangeBGM();
	OrganizeGridInfo();
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

		IOManager::PlaySfx("resource/sfx/build.wav", true);
		groundTileMap->UpdateTile(gridCoord);
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(0, -1));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(0, 1));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(1, 0));
		groundTileMap->UpdateTile(gridCoord + sf::Vector2i(-1, 0));

		OrganizeGridInfo();
		TileSort();
		return true;
	}

	return false;

}

void SceneGame::OrganizeGridInfo()
{
	auto x1 = gridInfo.begin();
	while (x1 != gridInfo.end())
	{
		if (x1->second.empty())
		{
			x1 = gridInfo.erase(x1);
			continue;
		}

		auto y = x1->second.begin();
		while (y != x1->second.end())
		{
			if (y->second.second.expired())
				y = x1->second.erase(y);
			else
				y++;
		}
		x1++;
	}

	auto x = unitOnGrid.begin();
	while (x != unitOnGrid.end())
	{
		if (x->second.empty())
		{
			x = unitOnGrid.erase(x);
			continue;
		}

		auto y = x->second.begin();
		while (y != x->second.end())
		{
			if (y->second.empty())
			{
				y = x->second.erase(y);
				continue;
			}

			auto it = y->second.begin();
			while (it != y->second.end())
			{
				if (it->expired())
					it = y->second.erase(it);
				else
					it++;
			}
			y++;
		}
		x++;
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

sf::Vector2i SceneGame::PosToGridCoord(const sf::Vector2f& position)
{
	sf::Vector2i gridCoord;
	if (position.x >= 0)
		gridCoord.x = position.x / gridSize.x;
	else
		gridCoord.x = floor(position.x / gridSize.x);

	if (position.y >= 0)
		gridCoord.y = position.y / gridSize.y;
	else
		gridCoord.y = floor(position.y / gridSize.y);

	return gridCoord;
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
	GameManager::lastGameName = city.mayorName;
}

void SceneGame::LoadGame()
{
	isLoading = true;
	DataManager::LoadMayor(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
	DataManager::LoadTile(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
	DataManager::LoadUnit(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
	std::dynamic_pointer_cast<SceneGameUI, Scene>(SceneManager::Get("SceneGameUI"))->Reset();
	GameManager::lastGameName = city.mayorName;
	void TileSort();
	ChangeBGM();
	gameOver = false;
	isLoading = false;
}

void SceneGame::LoadMayor(CITY city)
{
	this->city = city;
}

bool SceneGame::LoadObjectTile(const RCI& rci, const sf::Vector2i& gridCoord,
	const std::list<GAME_OBJECT_TAG>& tagList, GAME_OBJECT_TYPE type, float soundTimer, float soundDuration)
{
	if (gridInfo[gridCoord.x][gridCoord.y].first == GAME_OBJECT_TYPE::NONE)
	{
		gridInfo[gridCoord.x][gridCoord.y].first = type;
		if (type == GAME_OBJECT_TYPE::ROAD)
		{
			gridInfo[gridCoord.x][gridCoord.y].second = TileRoad::Create(This(), gridCoord, tagList);
		}
		else if (type >= GAME_OBJECT_TYPE::BUILDING && type < GAME_OBJECT_TYPE::BUILDING_END)
		{
			gridInfo[gridCoord.x][gridCoord.y].second = TileBuilding::Create(rci, This(), gridCoord, tagList, type, soundTimer, soundDuration);
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
	timeScale = 0.f;
	IOManager::BGMSyncPlay("resource/music/NightClose.wav", "resource/music/NightFar.wav");
}

void SceneGame::CameraInput(float timeDelta)
{
	if (isCameraFixed)
		return;

	//UI 숨기기
	if (IOManager::IsKeyDown(sf::Keyboard::F1))
	{
		if (isUIHide)
		{
			isUIHide = false;
			indicater->SetActive(true);
		}
		else
		{
			isUIHide = true;
			indicater->SetActive(false);
		}
	}
	if (isUIHide && IOManager::IsKeyDown(sf::Keyboard::Escape))
	{
		isUIHide = false;
		indicater->SetActive(true);
	}

	if (IOManager::IsKeyPress(sf::Keyboard::Q))
	{
		view.rotate(90.f * timeDelta);
		isCameraMoved = true;
	}
	if (IOManager::IsKeyPress(sf::Keyboard::E))
	{
		view.rotate(-90.f * timeDelta);
		isCameraMoved = true;
	}



	if (IOManager::IsKeyPress(sf::Keyboard::W))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(0.f, -zoomY * 0.5f * timeDelta));
		isCameraMoved = true;
	}
	if (IOManager::IsKeyPress(sf::Keyboard::S))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(0.f, zoomY * 0.5f * timeDelta));
		isCameraMoved = true;
	}
	if (IOManager::IsKeyPress(sf::Keyboard::A))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(-zoomY * 0.5f * timeDelta, 0.f));
		isCameraMoved = true;
	}
	if (IOManager::IsKeyPress(sf::Keyboard::D))
	{
		view.move(sf::Transform().rotate(view.getRotation()) * sf::Vector2f(zoomY * 0.5f * timeDelta, 0.f));
		isCameraMoved = true;
	}

	if (IOManager::GetWheelDelta() > 0)
	{
		view.zoom(1.f / 1.05f);
		zoomY *= 1.f / 1.05f;
		zoomRatio *= 1.f / 1.05f;
		isCameraMoved = true;
		SetBGMSync();
	}
	else if (IOManager::GetWheelDelta() < 0)
	{
		view.zoom(1.05f);
		zoomY *= 1.05f;
		zoomRatio *= 1.05f;
		isCameraMoved = true;
		SetBGMSync();
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
			isCameraMoved = true;
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
		zoomRatio = 1.00f;
		zoomY = view.getSize().y;
		view.setCenter(0.f, 0.f);
		isCameraMoved = true;
	}
}

void SceneGame::OnCameraMove()
{

	//Listener 위치 설정
	sf::Vector2f tempV = { 0.f, 1.f };
	tempV = sf::Transform().rotate(view.getRotation()).translate(tempV).transformPoint(tempV);
	sf::Listener::setUpVector(tempV.x, tempV.y, tilt - 1.f);
	tempV *= zoomY * (tilt - 1.f) * 0.05f;
	sf::Listener::setPosition(view.getCenter().x + tempV.x, view.getCenter().y + tempV.y, zoomRatio * 400.f);

	sf::Vector2i unMute = PosToGridCoord({ sf::Listener::getPosition().x, sf::Listener::getPosition().y });
	if (unMute != preListenerGridCoord)
	{
		unMute.x -= soundRadius + 1;
		unMute.y -= soundRadius + 1;
		for (int x = 0; x < (soundRadius + 1) * 2; x++)
		{
			for (int y = 0; y < (soundRadius + 1) * 2; y++)
			{
				if (x == 0 || x == (soundRadius + 1) * 2 - 1 || y == 0 || y == (soundRadius + 1) * 2 - 1)
				{
					if (!gridInfo[unMute.x + x][unMute.y + y].second.expired())
						gridInfo[unMute.x + x][unMute.y + y].second.lock()->SetMute(true);
					else
						gridInfo[unMute.x + x].erase(unMute.y + y);

					for (auto& ptr : unitOnGrid[unMute.x + x][unMute.y + y])
					{
						if (!ptr.expired())
						{
							ptr.lock()->SetMute(true);
						}
					}
				}
				else
				{
					if (!gridInfo[unMute.x + x][unMute.y + y].second.expired())
						gridInfo[unMute.x + x][unMute.y + y].second.lock()->SetMute(false);
					else
						gridInfo[unMute.x + x].erase(unMute.y + y);

					for (auto& ptr : unitOnGrid[unMute.x + x][unMute.y + y])
					{
						if (!ptr.expired())
						{
							ptr.lock()->SetMute(false);
						}
					}
				}
			}
		}
	}
	preListenerGridCoord = PosToGridCoord({ sf::Listener::getPosition().x, sf::Listener::getPosition().y });

	sf::RenderWindow& window = GameManager::GetWindow();
	frustum.nearPlane = tool::To3D(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, window.getSize().y), view));
	frustum.farPlane = tool::To3D(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, 0), view));
	frustum.leftPlane = tool::To3D(window.mapPixelToCoords(sf::Vector2i(0, window.getSize().y / 2), view));
	frustum.rightPlane = tool::To3D(window.mapPixelToCoords(sf::Vector2i(window.getSize().x, window.getSize().y / 2), view));

	TileSort();
	if (timeScale == 0.f)
		UnitSort();
}

void SceneGame::TileSort()
{
	doDrawLayerSort = true;
	sf::Vector3f plane;
	sf::Vector3f building;
	for (auto& x : gridInfo)
	{
		for (auto& y : x.second)
		{
			if (y.second.second.expired())
				continue;

			if (y.second.first == GAME_OBJECT_TYPE::ROAD)
			{
				if (!GameManager::DoDebug())
					continue;
			}

			auto thisTile = y.second.second.lock();

			plane = frustum.nearPlane - tool::To3D(view.getCenter());
			building = tool::To3D(thisTile->GetGridCenterPos()) - frustum.nearPlane;
			float onN = tool::OnPlane(plane, building);
			if (onN > 0.f)
			{
				thisTile->SetDrawDeep(0.f);
				thisTile->SetShow(false);
				continue;
			}

			plane = frustum.farPlane - tool::To3D(view.getCenter());
			building = tool::To3D(thisTile->GetGridCenterPos()) - frustum.farPlane;
			float onF = tool::OnPlane(plane, building);
			if (onF > 0.f)
			{
				thisTile->SetDrawDeep(0.f);
				thisTile->SetShow(false);
				continue;
			}

			plane = frustum.leftPlane - tool::To3D(view.getCenter());
			building = tool::To3D(thisTile->GetGridCenterPos()) - frustum.leftPlane;
			float onL = tool::OnPlane(plane, building);
			if (onL > 0.f)
			{
				thisTile->SetDrawDeep(0.f);
				thisTile->SetShow(false);
				continue;
			}

			plane = frustum.rightPlane - tool::To3D(view.getCenter());
			building = tool::To3D(thisTile->GetGridCenterPos()) - frustum.rightPlane;
			float onR = tool::OnPlane(plane, building);
			if (onR > 0.f)
			{
				thisTile->SetDrawDeep(0.f);
				thisTile->SetShow(false);
				continue;
			}
			else
			{
				thisTile->SetDrawDeep(-onN);
				thisTile->SetShow(true);
			}
		}
	}
}

void SceneGame::UnitSort()
{
	doDrawLayerSort = true;
	for (auto& x : unitOnGrid)
	{
		for (auto& y : x.second)
		{
			for (auto& units : y.second)
			{
				if (units.expired())
					continue;
				units.lock()->AutoDrawDeep();
			}
		}
	}
}

void SceneGame::SetMousePosGrid()
{
	mousePosGrid = PosToGridCoord(mousePosWorld);
}

void SceneGame::SetBGMSync()
{
	if (zoomRatio < 0.35f)
	{
		IOManager::SetBGMCh1Volume(0.f);
		IOManager::SetBGMCh2Volume(0.f);
	}
	else if (zoomRatio <= 1.35f)
	{
		IOManager::SetBGMCh1Volume(100.f * (zoomRatio - 0.35f));
		IOManager::SetBGMCh2Volume(0.f);
	}
	else if (zoomRatio <= 2.35f)
	{
		IOManager::SetBGMCh1Volume(100.f * (2.35f - zoomRatio));
		IOManager::SetBGMCh2Volume(100.f * (zoomRatio - 1.35f));
	}
	else
	{
		IOManager::SetBGMCh1Volume(0.f);
		IOManager::SetBGMCh2Volume(100.f);
	}
}

void SceneGame::ChangeBGM()
{
	if ((gameOver || !isMiddleCity) && ObjectUnit::GetUnitCount() >= 100)
	{
		isMiddleCity = true;
		IOManager::BGMSyncPlay("resource/music/MiddleClose.wav", "resource/music/MiddleFar.wav", true, true);
	}
	else if ((gameOver || isMiddleCity) && ObjectUnit::GetUnitCount() <= 50)
	{
		isMiddleCity = false;
		IOManager::BGMSyncPlay("resource/music/LittleClose.wav", "resource/music/LittleFar.wav", true, true);
	}
	SetBGMSync();
}
