#include "pch.h"
#include "SceneGame.h"
#include "ObjectIndicater.h"
#include "_Include_Tile.h"
#include "ObjectUnit.h"
#include "CsvFile.h"
#include <DataManager.h>

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

	//���õ� Ÿ�� ǥ�ÿ� (�ӽ�)
	Scene::Init();
	//�ʱ� ī�޶� ��ġ -> TODO ���� ����� �����Ͽ� �ٽ� �ҷ��� �� �ֵ���
	viewZoomTarget = view.getSize();

	background.setFillColor({ 20,120,20,255 });
	background.setSize(view.getSize());
	tool::SetOrigin(background, ORIGIN::M);

	groundTileMap = ObjectTileMap::Create(This());

}

void SceneGame::PreUpdate(float timeDelta, float timeScale)
{
	Scene::PreUpdate(timeDelta, timeScale);
	SetMousePosGrid();

	citizenTimer += timeDelta * timeScale;
	if (GM_RCI.Instance().LeftRegidence() > 0 && citizenTimer >= citizenInterval)
	{
		citizenTimer = 0.f;
		SceneGame::AddUnit(ObjectUnit::Create(This()));
	}

	//TESTCODE �� ������ ���� �׽�Ʈ �ڵ�

	if (IOManager::IsKeyPress(sf::Keyboard::Q))
	{
		view.rotate(45.f * timeDelta);
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
		viewZoom += 0.1;
		viewZoomTarget /= 1 + viewZoom;
	}
	else if (IOManager::GetWheelDelta() < 0)
	{
		viewZoom += 0.1;
		viewZoomTarget *= 1 + viewZoom;
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
			tilt = startTilt - GameManager::GetMousePosWindow().y;
			view.setSize(view.getSize().x, view.getSize().y + tilt);
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
	}


	/*if (IOManager::IsKeyDown(sf::Mouse::Left))
	{
		if (gridInfo[mousePosGrid.x][mousePosGrid.y].second.expired())
			CreateObjectTile(GAME_OBJECT_TYPE::BUILDING, mousePosGrid);
		else
			DeleteObjectTile(mousePosGrid);
	}
	if (IOManager::IsKeyDown(sf::Mouse::Right))
	{
		if (gridInfo[mousePosGrid.x][mousePosGrid.y].second.expired())
			CreateObjectTile(GAME_OBJECT_TYPE::GROUND, mousePosGrid);
		else
			DeleteObjectTile(mousePosGrid);
	}
	if (IOManager::IsKeyDown(sf::Keyboard::Num2))
	{
		if (gridInfo[mousePosGrid.x][mousePosGrid.y].second.expired())
			CreateObjectTile(GAME_OBJECT_TYPE::ROAD, mousePosGrid);
		else
			DeleteObjectTile(mousePosGrid);
	}
	if (IOManager::IsKeyDown(sf::Keyboard::Num3))
	{



	}
	if (IOManager::IsKeyDown(sf::Keyboard::F5))
	{
		OrganizeGridInfo();
	}

	if (IOManager::IsKeyDown(sf::Keyboard::F))
	{
		SaveGame();
	}
	if (IOManager::IsKeyDown(sf::Keyboard::G))
	{
		LoadGame();
	}
	if (IOManager::IsKeyDown(sf::Keyboard::R))
	{
		Reset();
	}*/
	background.setPosition(view.getCenter());
}

void SceneGame::Update(float timeDelta, float timeScale)
{
	if (view.getSize().x > viewZoomTarget.x)
	{
		view.zoom(1 / (1 + viewZoom * timeDelta * timeScale));

		if (abs(viewZoomTarget.x - view.getSize().x) < 1 / (1 + viewZoom * timeDelta * timeScale))
		{
			view.setSize(viewZoomTarget);
			viewZoom = 0.f;
		}
	}
	else if (view.getSize().x < viewZoomTarget.x)
	{
		view.zoom(1 + viewZoom * timeDelta * timeScale);

		if (abs(viewZoomTarget.x - view.getSize().x) < 1 + viewZoom * timeDelta * timeScale)
		{
			view.setSize(viewZoomTarget);
			viewZoom = 0.f;
		}
	}
	Scene::Update(timeDelta, timeScale);
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
	gridInfo.clear();
	unitOnGrid.clear();

	gameObjectList.clear();
	drawList.clear();

	AddObject(groundTileMap);
	Scene::Reset();

	AddObject(std::make_shared<ObjectIndicater>(This(), GAME_OBJECT_TYPE::NONE))->Init();
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

bool SceneGame::CreateObjectTile(GAME_OBJECT_TYPE type, const sf::Vector2i& gridCoord)
{
	if (gridInfo[gridCoord.x][gridCoord.y].second.expired())
	{
		gridInfo[gridCoord.x][gridCoord.y].first = type;
		switch (type)
		{
		case GAME_OBJECT_TYPE::ROAD:
			gridInfo[gridCoord.x][gridCoord.y].second = TileRoad::Create(This(), gridCoord);
			break;
		case GAME_OBJECT_TYPE::HOME:
		{
			RCI rci = { GameManager::RandomRange(1, 3),0,0 };
			gridInfo[gridCoord.x][gridCoord.y].second = TileBuilding::Create(rci, This(), gridCoord);
			break;
		}
		case GAME_OBJECT_TYPE::WORK_PLACE:
		{		
			RCI rci = { 0,0,GameManager::RandomRange(1,3) };
			gridInfo[gridCoord.x][gridCoord.y].second = TileBuilding::Create(rci, This(), gridCoord);
			break;
		}
		default:
			return false;
			break;
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

void SceneGame::DeleteObjectTile(const sf::Vector2i& gridCoord)
{
	if (gridInfo[gridCoord.x][gridCoord.y].second.expired())
		return;
	auto& adjacent = gridInfo[gridCoord.x][gridCoord.y].second.lock()->GetAdjacent();
	DeleteObject(gridInfo[gridCoord.x][gridCoord.y].second.lock()->GetKey());
	gridInfo[gridCoord.x][gridCoord.y].first = GAME_OBJECT_TYPE::NONE;
	gridInfo[gridCoord.x][gridCoord.y].second.reset();

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
					if (tempPtr->GetNextTile().expired())
						tempPtr->SetPosition({ sf::Vector2f(teleport).x * gridSize.x + gridSize.x * 0.5f,
							sf::Vector2f(teleport).y * gridSize.y + gridSize.y * 0.5f });
					else
						tempPtr->SetPosition(tempPtr->GetNextTile().lock()->GetGridCenterPos());
				}
			}
		}
	}
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
	DataManager::SaveTile(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
	DataManager::SaveUnit(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
}

void SceneGame::LoadGame()
{
	DataManager::LoadTile(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
	DataManager::LoadUnit(std::dynamic_pointer_cast<SceneGame, Scene>(This()));
}

bool SceneGame::LoadObjectTile(GAME_OBJECT_TYPE type, const sf::Vector2i& gridCoord
	, const std::list<GAME_OBJECT_TAG>& tagList, const sf::IntRect& rect, const RCI& rci)
{
	if (gridInfo[gridCoord.x][gridCoord.y].first == GAME_OBJECT_TYPE::NONE)
	{
		gridInfo[gridCoord.x][gridCoord.y].first = type;
		switch (type)
		{
		case GAME_OBJECT_TYPE::ROAD:
			gridInfo[gridCoord.x][gridCoord.y].second = TileRoad::Create(This(), gridCoord, tagList, rect);
			break;
		case GAME_OBJECT_TYPE::HOME:
		{
			gridInfo[gridCoord.x][gridCoord.y].second = TileBuilding::Create(This(), gridCoord, tagList, rect, rci);
			break;
		}
		case GAME_OBJECT_TYPE::WORK_PLACE:
		{
			gridInfo[gridCoord.x][gridCoord.y].second = TileBuilding::Create(This(), gridCoord, tagList, rect, rci);
			break;
		}
		default:
			return false;
			break;
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