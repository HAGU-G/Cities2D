#include "pch.h"
#include "SceneGame.h"
#include "Test/ObjectTest.h"
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
	//선택된 타일 표시용 (임시)
	Scene::Init();
	//초기 카메라 위치 -> TODO 게임 저장시 저장하여 다시 불러올 수 있도록
	//view.setCenter(worldCenter);

	AddObject(std::make_shared<ObjectTest>(This(), GAME_OBJECT_TYPE::NONE))->Init();
	groundTileMap = ObjectTileMap::Create(This());

}

void SceneGame::PreUpdate(float timeDelta, float timeScale)
{
	Scene::PreUpdate(timeDelta, timeScale);
	SetMousePosGrid();

	//TESTCODE 이 밑으로 전부 테스트 코드

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


	if (IOManager::IsKeyDown(sf::Mouse::Left))
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

		SceneGame::AddUnit(ObjectUnit::Create(This()));

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
	}


}

void SceneGame::Update(float timeDelta, float timeScale)
{
	Scene::Update(timeDelta, timeScale);
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneGame::Reset()
{
	gridInfo.clear();
	unitOnGrid.clear();

	auto it = gameObjectList.begin();
	while (it != gameObjectList.end())
	{
		if (it->second->GetGameObjectType() >= GAME_OBJECT_TYPE::TILE && it->second->GetGameObjectType() <= GAME_OBJECT_TYPE::UNIT_END)
		{
			auto drawIt = drawList.begin();
			while (drawIt != drawList.end())
			{
				if (drawIt->first == it->first)
				{
					drawList.erase(drawIt);
					break;
				}
				{
					drawIt++;
				}
			}
			it = gameObjectList.erase(it);
		}
		else
		{
			it++;
		}
	}

	Scene::Reset();

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
	if (gridInfo[gridCoord.x][gridCoord.y].first == GAME_OBJECT_TYPE::NONE)
	{
		switch (type)
		{
		case GAME_OBJECT_TYPE::ROAD:
			gridInfo[gridCoord.x][gridCoord.y].first = type;
			gridInfo[gridCoord.x][gridCoord.y].second = TileRoad::Create(This(), gridCoord);
			groundTileMap->UpdateTile(gridCoord);
			return true;
			break;
		case GAME_OBJECT_TYPE::BUILDING:
		{
			RCI rci = { GameManager::RandomRange(1, 3),0,0 };
			gridInfo[gridCoord.x][gridCoord.y].first = type;
			gridInfo[gridCoord.x][gridCoord.y].second = TileBuilding::Create(rci, This(), gridCoord);
			groundTileMap->UpdateTile(gridCoord);
			return true;
			break;
		}
		case GAME_OBJECT_TYPE::GROUND:
		{
			RCI rci = { 0,0,GameManager::RandomRange(1,3) };
			gridInfo[gridCoord.x][gridCoord.y].first = GAME_OBJECT_TYPE::BUILDING;
			gridInfo[gridCoord.x][gridCoord.y].second = TileBuilding::Create(rci, This(), gridCoord);
			groundTileMap->UpdateTile(gridCoord);
			return true;
			break;
		}
		default:
			break;
		}

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
	DeleteObject(gridInfo[gridCoord.x][gridCoord.y].second.lock()->GetKey());
	gridInfo[gridCoord.x][gridCoord.y].first = GAME_OBJECT_TYPE::NONE;
	gridInfo[gridCoord.x][gridCoord.y].second.reset();

	groundTileMap->UpdateTile(gridCoord);

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
		switch (type)
		{
		case GAME_OBJECT_TYPE::ROAD:
			gridInfo[gridCoord.x][gridCoord.y].first = type;
			gridInfo[gridCoord.x][gridCoord.y].second = TileRoad::Create(This(), gridCoord, tagList, rect);
			groundTileMap->UpdateTile(gridCoord);
			return true;
			break;
		case GAME_OBJECT_TYPE::BUILDING:
		{
			gridInfo[gridCoord.x][gridCoord.y].first = type;
			gridInfo[gridCoord.x][gridCoord.y].second = TileBuilding::Create(This(), gridCoord, tagList, rect, rci);
			groundTileMap->UpdateTile(gridCoord);
			return true;
			break;
		}
		default:
			break;
		}

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