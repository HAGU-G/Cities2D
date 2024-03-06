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
	srand(time(NULL) + unsigned(this) + scene.lock()->GetMousePosWorld().x * sceneGame.lock()->GetMousePosWorld().y);
	findInterval = tool::RandomBetween(5.f, 10.f);
	Reset();
}

void ObjectUnit::Update(float timeDelta, float timeScale)
{
	if (!hasHome)
	{
		if (findTimer >= findInterval)
		{
			findTimer = 0.f;
			if(!FindHome())
				patience--;
		}
		else
		{
			findTimer += timeDelta * timeScale;
		}
	}
	else if (home.expired())
	{
		hasHome = false;
		GM_RCI.UseRegidence(-1);
	}

	if (patience <= 0)
	{
		scene.lock()->DeleteObject(GetKey());
	}
}

void ObjectUnit::Reset()
{
	home.reset();
	workPlace.reset();
	GameObject::Reset();
}

void ObjectUnit::Release()
{
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

void ObjectUnit::SetHome(std::weak_ptr<TileBuilding> building)
{
	if (!home.expired())
	{
		GM_RCI.UseRegidence(-1);
		building.lock()->MoveOut(GetKey());
	}
	home = building;
	hasHome = true;
}

void ObjectUnit::SetWorkPlace(std::weak_ptr<TileBuilding> building)
{
	if (!workPlace.expired())
	{
		GM_RCI.UseIndustry(-1);
		//building.lock()->MoveOut(GetKey());
		//구현필요.
	}
	workPlace = building;
	hasworkPlace = true;
}
