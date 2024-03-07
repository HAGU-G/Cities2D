#include "pch.h"
#include "ObjectUnit.h"
#include "TileBuilding.h"
#include "SceneGame.h"
#include "Scene.h"

ObjectUnit::ObjectUnit(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType)
	:GameObject(scene, objectType)
{
}

ObjectUnit::~ObjectUnit()
{
	Release();
}

void ObjectUnit::Init()
{
	sceneGame = std::dynamic_pointer_cast<SceneGame, Scene>(scene.lock());
	findInterval = tool::RandomBetween(5.f, 10.f);
	Reset();
}

void ObjectUnit::Update(float timeDelta, float timeScale)
{
	if (!hasHome) //집이 없을 때
	{
		if (findTimer >= findInterval)
		{
			findTimer = 0.f;
			if (FindHome())
				patience++;
			else
				patience--;
		}
		else
		{
			findTimer += timeDelta * timeScale;
		}
	}
	else //집이 있을 때
	{
		if (home.expired()) //집이 부숴졌을 때
		{
			hasHome = false;
			GM_RCI.UseRegidence(-1);
		}
	}

	if (!hasworkPlace) //직장이 없을 때
	{
		if (hasHome)
		{
			if (findTimer >= findInterval)
			{
				findTimer = 0.f;
				if (FindWorkSpace())
					patience++;
				else
					patience--;
			}
			else
			{
				findTimer += timeDelta * timeScale;
			}
		}
	}
	else
	{
		if (workPlace.expired()) //회사가 부숴졌을 때
		{
			hasworkPlace = false;
			GM_RCI.UseIndustry(-1);
		}
	}
}

void ObjectUnit::PostUpdate(float timeDelta, float timeScale)
{
	if (patience <= 0)
	{
		scene.lock()->DeleteObject(GetKey());
	}
}

void ObjectUnit::Reset()
{
	if (!home.expired())
	{
		home.lock()->MoveOut(GetKey());
		GM_RCI.UseRegidence(-1);
	}
	if (!workPlace.expired())
	{
		workPlace.lock()->Quit(GetKey());
		GM_RCI.UseIndustry(-1);
	}
	home.reset();
	workPlace.reset();
	GameObject::Reset();
}

void ObjectUnit::Release()
{
	if (!home.expired())
	{
		home.lock()->MoveOut(GetKey());
		GM_RCI.UseRegidence(-1);
	}
	if (!workPlace.expired())
	{
		workPlace.lock()->Quit(GetKey());
		GM_RCI.UseIndustry(-1);
	}
	home.reset();
	workPlace.reset();
}

std::shared_ptr<ObjectUnit> ObjectUnit::Create(std::weak_ptr<Scene> scene)
{
	std::shared_ptr<ObjectUnit> objectUnit = std::make_shared<ObjectUnit>(scene, GAME_OBJECT_TYPE::CITIZEN);
	scene.lock()->AddObject(objectUnit);
	objectUnit->Init();
	return objectUnit;
}

bool ObjectUnit::FindHome()
{
	const GridInfo& tiles = sceneGame.lock()->GetGridInfo();

	for (auto& x : tiles)
	{
		for (auto& y : x.second)
		{
			if (y.second.first != GAME_OBJECT_TYPE::BUILDING)
				continue;

			if (std::dynamic_pointer_cast<TileBuilding, ObjectTile>(y.second.second)
				->MoveIn(std::dynamic_pointer_cast<ObjectUnit, GameObject>(This())) == true)
				return true;
		}
	}
	return false;
}

bool ObjectUnit::FindWorkSpace()
{
	//가까운 직장을 찾게 변경할 예정
	const GridInfo& tiles = sceneGame.lock()->GetGridInfo();

	for (auto& x : tiles)
	{
		for (auto& y : x.second)
		{
			if (y.second.first != GAME_OBJECT_TYPE::BUILDING)
				continue;

			if (std::dynamic_pointer_cast<TileBuilding, ObjectTile>(y.second.second)
				->Join(std::dynamic_pointer_cast<ObjectUnit, GameObject>(This())) == true)
				return true;
		}
	}
	return false;
}

void ObjectUnit::SetHome(std::weak_ptr<TileBuilding> building)
{
	if (!home.expired())
	{
		home.lock()->MoveOut(GetKey());
		GM_RCI.UseRegidence(-1);
	}
	home = building;
	hasHome = true;
}

void ObjectUnit::SetWorkPlace(std::weak_ptr<TileBuilding> building)
{
	if (!workPlace.expired())
	{
		workPlace.lock()->Quit(GetKey());
		GM_RCI.UseIndustry(-1);
	}
	workPlace = building;
	hasworkPlace = true;
}
