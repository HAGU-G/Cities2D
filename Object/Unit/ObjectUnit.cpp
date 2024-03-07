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
	Reset();
}

void ObjectUnit::Update(float timeDelta, float timeScale)
{
	if(sceneGame.lock()->GetTileInfo(gridCoord))//TODO 작성중
	UpdateHome(timeDelta, timeScale);
	UpdateWorkSpace(timeDelta, timeScale);
	LifeCycle(timeDelta, timeScale);
}

void ObjectUnit::PostUpdate(float timeDelta, float timeScale)
{
	if (patience <= 0)
	{
		scene.lock()->DeleteObject(GetKey());
	}
}

void ObjectUnit::Draw(sf::RenderWindow& window)
{
	if (status >= STATUS::WALK && status < STATUS::WALK_END)
	{
		window.draw(tempRender);
	}
}

void ObjectUnit::Reset()
{
	findInterval = tool::RandomBetween(0.1f, 1.f);

	tempRender.setRadius(tool::RandomBetween(5.f, 7.f));
	tempRender.setFillColor(sf::Color(tool::RandomBetween(0, 255), tool::RandomBetween(0, 255), tool::RandomBetween(0, 255), 255));
	tempRender.setOrigin(tempRender.getLocalBounds().getSize() * 0.5f);

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

void ObjectUnit::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	tempRender.setPosition(position);

	sf::Vector2f gridSize = sceneGame.lock()->GetGridSize();
	if (position.x >= 0)
		gridCoord.x = position.x / gridSize.x;
	else
		gridCoord.x = floor(position.x / gridSize.x);

	if (position.y >= 0)
		gridCoord.y = position.y / gridSize.y;
	else
		gridCoord.y = floor(position.y / gridSize.y);

	gridCenterPos.x = gridCoord.x * gridSize.x + gridSize.x * 0.5f;
	gridCenterPos.y = gridCoord.y * gridSize.y + gridSize.y * 0.5f;


}

void ObjectUnit::UpdateHome(float timeDelta, float timeScale)
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
}

void ObjectUnit::UpdateWorkSpace(float timeDelta, float timeScale)
{
	if (!hasWorkPlace) //직장이 없을 때
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
			hasWorkPlace = false;
			GM_RCI.UseIndustry(-1);
		}
	}
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

	pathToWorkPlace = ObjectTile::FindShortPath(home, GAME_OBJECT_TAG::I);

	if (pathToWorkPlace.empty())
		return false;

	std::dynamic_pointer_cast<TileBuilding, ObjectTile>(sceneGame.lock()->GetTileInfo(pathToWorkPlace.back()).second)
		->Join(std::dynamic_pointer_cast<ObjectUnit, GameObject>(This()));
	return true;
}

void ObjectUnit::LifeCycle(float timeDelta, float timeScale)
{
	sf::Vector2f gridSize = sceneGame.lock()->GetGridSize();
	switch (status)
	{
	case STATUS::NONE:
		break;
	case STATUS::HOME:
		if (hasWorkPlace)
		{
			if (lifeTimer < lifeInterval)
			{
				lifeTimer += timeDelta * timeScale;
			}
			else
			{
				lifeTimer = 0.f;
				if (home.lock() == workPlace.lock())
				{
					status = STATUS::WORK_PLACE;
				}
				else
				{
					walkPath = pathToWorkPlace;
					status = STATUS::TO_WORK_PLACE;
				}

			}
		}
		break;
	case STATUS::TO_WORK_PLACE:
		if (hasWorkPlace)
		{
			float goalX = walkPath.front().x * gridSize.x + gridSize.x * 0.5f;
			float goalY = walkPath.front().y * gridSize.y + gridSize.y * 0.5f;
			direction.x = goalX - position.x;
			direction.y = goalY - position.y;
			tool::Normalize(direction);
			SetPosition(position + direction * speed * timeDelta * timeScale);
			if (tool::Distance(position, sf::Vector2f(goalX, goalY)) <= 1.f)
			{
				SetPosition({ goalX, goalY });
				walkPath.pop_front();
			}
			if (tool::Distance(position, workPlace.lock()->GetGridCenterPos()) <= 1.f)
			{
				status = STATUS::WORK_PLACE;
				SetPosition(workPlace.lock()->GetGridCenterPos());
				walkPath.clear();
			}
		}
		else if (hasHome)
		{

			walkPath = ObjectTile::FindShortPath(home, sceneGame.lock()->GetTileInfo(gridCoord).second);
			if (walkPath.empty())
			{
				status = STATUS::NONE;
			}
			else
			{
				status = STATUS::TO_HOME;
			}
		}
		break;
	case STATUS::WORK_PLACE:
		if (hasHome)
		{
			if (lifeTimer < lifeInterval)
			{
				lifeTimer += timeDelta * timeScale;
			}
			else
			{
				lifeTimer = 0.f;
				if (home.lock() == workPlace.lock())
				{
					status = STATUS::HOME;
				}
				else
				{
					walkPath = pathToWorkPlace;
					status = STATUS::TO_HOME;
				}
			}
		}
		break;
	case STATUS::TO_HOME:
		if (hasHome)
		{
			float goalX = walkPath.back().x * gridSize.x + gridSize.x * 0.5f;
			float goalY = walkPath.back().y * gridSize.y + gridSize.y * 0.5f;
			direction.x = goalX - position.x;
			direction.y = goalY - position.y;
			tool::Normalize(direction);
			SetPosition(position + direction * speed * timeDelta * timeScale);
			if (tool::Distance(position, sf::Vector2f(goalX, goalY)) <= 1.f)
			{
				SetPosition({ goalX, goalY });
				walkPath.pop_back();
			}
			if (tool::Distance(position, home.lock()->GetGridCenterPos()) <= 1.f)
			{
				status = STATUS::HOME;
				SetPosition(home.lock()->GetGridCenterPos());
				walkPath.clear();
			}
		}
		else
		{
			status = STATUS::NONE;
		}
		break;
	default:
		break;
	}

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

	if (!isMoveToCity)
	{
		isMoveToCity = true;
		SetPosition(home.lock()->GetGridCenterPos());
		status = STATUS::HOME;
	}
}

void ObjectUnit::SetWorkPlace(std::weak_ptr<TileBuilding> building)
{
	if (!workPlace.expired())
	{
		workPlace.lock()->Quit(GetKey());
		GM_RCI.UseIndustry(-1);
	}
	workPlace = building;
	hasWorkPlace = true;
}
