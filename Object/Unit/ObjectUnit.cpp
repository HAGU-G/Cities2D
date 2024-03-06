#include "pch.h"
#include "ObjectUnit.h"
#include "TileBuilding.h"
#include "SceneGame.h"

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
	findInterval = tool::Random(5.f, 10.f);
	Reset();
}

void ObjectUnit::Update(float timeDelta, float timeScale)
{
	if (findTimer >= findInterval)
	{
		findTimer = 0.f;
		FindHome();
	}
	else
	{
		findTimer += timeDelta * timeScale;
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

void ObjectUnit::FindHome()
{
	const GridInfo& tiles = sceneGame.lock()->GetGridInfo();

	for (auto& x : tiles)
	{
		for (auto& y : x.second)
		{
			if (y.second.first != GAME_OBJECT_TYPE::BUILDING)
				continue;

			if (std::dynamic_pointer_cast<TileBuilding, GameObject>(y.second.second)
				->MoveIn(std::dynamic_pointer_cast<ObjectUnit, GameObject>(This())) == true)
				return;
		}
	}
}

void ObjectUnit::SetHome(std::weak_ptr<TileBuilding> building)
{
	home = building;
}

void ObjectUnit::SetWorkPlace(std::weak_ptr<TileBuilding> building)
{
	workPlace = building;
}
