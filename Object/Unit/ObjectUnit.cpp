#include "pch.h"
#include "ObjectUnit.h"
#include "TileBuilding.h"
#include "SceneGame.h"
#include "Scene.h"

size_t ObjectUnit::citizenCount = 0;

void ObjectUnit::NoHome()
{
	home.reset();
	hasHome = false;
	GM_RCI.UseRegidence(-1);
}

void ObjectUnit::NoWorkPlace()
{
	workPlace.reset();
	hasWorkPlace = false;
	GM_RCI.UseIndustry(-1);
}

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
}

void ObjectUnit::PreUpdate(float timeDelta, float timeScale)
{
	UpdateHome(timeDelta, timeScale);
	UpdateWorkSpace(timeDelta, timeScale);
}

void ObjectUnit::Update(float timeDelta, float timeScale)
{

	if (isCitizen)
	{
		GridUpdate();
		LifeCycle(timeDelta, timeScale);
	}
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
	findInterval = GameManager::RandomRange(0.1f, 1.f);

	tempRender.setRadius(GameManager::RandomRange(5.f, 7.f));
	tempRender.setFillColor(sf::Color(GameManager::RandomRange(0, 255), GameManager::RandomRange(0, 255), GameManager::RandomRange(0, 255), 255));
	tempRender.setOrigin(tempRender.getLocalBounds().getSize() * 0.5f);
	tempRender.setOutlineColor(sf::Color::Black);
	tempRender.setOutlineThickness(1);

	ResetHome();
	ResetWorkPlace();

	NoCitizen();
	status = STATUS::NONE;

	pathToWorkPlace.clear();
	walkPath.clear();
	isMoving = false;

	nextTile.reset();
	startingPoint.reset();
	destination.reset();

	GameObject::Reset();
}

void ObjectUnit::Release()
{
	GameObject::Release();
	ResetHome();
	ResetWorkPlace();
	NoCitizen();
	sceneGame.reset();
	nextTile.reset();
	startingPoint.reset();
	destination.reset();
}

std::shared_ptr<ObjectUnit> ObjectUnit::Create(std::weak_ptr<Scene> scene)
{
	std::shared_ptr<ObjectUnit> objectUnit = std::make_shared<ObjectUnit>(scene, GAME_OBJECT_TYPE::CITIZEN);
	scene.lock()->AddObject(objectUnit);
	objectUnit->Init();
	objectUnit->Reset();
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

void ObjectUnit::GridUpdate()
{
	std::shared_ptr<SceneGame> sceneGame = this->sceneGame.lock();
	std::shared_ptr<ObjectTile> currentTile = sceneGame->GetTileInfo(gridCoord).second.lock();
	const std::list<GAME_OBJECT_TAG>& tagList = currentTile->GetGameObjectTagList();
	if (sceneGame->GetTileInfo(gridCoord).second.expired()
		|| (std::find(tagList.begin(), tagList.end(), GAME_OBJECT_TAG::MOVEABLE) == tagList.end()
			&& (currentTile != home.lock() && currentTile != workPlace.lock())
			))
	{
		if (hasHome && hasWorkPlace)
		{
			pathToWorkPlace = ObjectTile::FindShortPath(home, workPlace);
			if (home.lock() != workPlace.lock() && pathToWorkPlace.empty())
			{
				if (status == STATUS::TO_HOME)
				{
					ResetHome();
				}
				else if (status == STATUS::TO_WORK_PLACE)
				{
					ResetWorkPlace();
				}
				else
				{
					Reset();
				}
			}
		}
		else if (hasHome)
		{
			CheckHome();
		}
		else if (hasWorkPlace)
		{
			CheckWorkPlace();
		}
		else
		{
			Reset();
		}
	}
	else if (preGridCoord != gridCoord)
	{
		const std::shared_ptr<ObjectUnit>& tempPtr = std::dynamic_pointer_cast<ObjectUnit, GameObject>(This());

		sceneGame->unitOnGrid[preGridCoord.x][preGridCoord.y]
			.remove_if(
				[tempPtr](std::weak_ptr<ObjectUnit> ptr)
				{
					return tempPtr.get() == ptr.lock().get();
				});

		sceneGame->unitOnGrid[gridCoord.x][gridCoord.y]
			.push_back(std::dynamic_pointer_cast<ObjectUnit, GameObject>(This()));
		preGridCoord = gridCoord;
	}
}

void ObjectUnit::UpdateHome(float timeDelta, float timeScale)
{
	if (!hasHome) //집이 없을 때
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
	else //집이 있을 때
	{
		if (home.expired()) //집이 없어졌을 때
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
				FindWorkSpace();
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

void ObjectUnit::BeCitizen()
{
	if (!isCitizen)
	{
		isCitizen = true;
		citizenCount++;
	}
}

void ObjectUnit::NoCitizen()
{
	if (isCitizen)
	{
		isCitizen = false;
		citizenCount--;
	}
}

bool ObjectUnit::FindHome()
{
	if (!isCitizen)
	{
		const GridInfo& tiles = sceneGame.lock()->GetGridInfo();

		for (auto& x : tiles)
		{
			for (auto& y : x.second)
			{
				if (y.second.first < GAME_OBJECT_TYPE::BUILDING || y.second.first >= GAME_OBJECT_TYPE::BUILDING_END)
					continue;

				std::shared_ptr<TileBuilding> home = C_TILE_BUILDING(y.second.second.lock());
				if (home->CanUseR(C_OBJECT_UNIT(This())))
				{
					patience++;
					home->UseR(C_OBJECT_UNIT(This()));
					SetHome(home);
					return true;
				}
			}
		}
	}
	else
	{
		std::deque<sf::Vector2i> pathToHome = ObjectTile::FindShortPath(sceneGame.lock()->GetTileInfo(gridCoord).second, GAME_OBJECT_TAG::R);

		if (pathToHome.empty())
		{
			patience--;
			return false;
		}

		if (sceneGame.lock()->GetTileInfo(pathToHome.back()).second.expired())
		{
			patience--;
			return false;
		}

		std::shared_ptr<TileBuilding> home = C_TILE_BUILDING(sceneGame.lock()->GetTileInfo(pathToHome.back()).second.lock());
		if (home->CanUseR(C_OBJECT_UNIT(This())))
		{
			patience++;
			home->UseR(C_OBJECT_UNIT(This()));
			SetHome(home);
			return true;
		}

	}

	patience++;
	return false;
}

bool ObjectUnit::FindWorkSpace()
{
	pathToWorkPlace = ObjectTile::FindShortPath(home, GAME_OBJECT_TAG::I);

	if (pathToWorkPlace.empty())
	{
		patience--;
		return false;
	}
	else
	{
		std::shared_ptr<TileBuilding> workPlace = C_TILE_BUILDING(sceneGame.lock()->GetTileInfo(pathToWorkPlace.back()).second.lock());
		if (workPlace->CanUseI(C_OBJECT_UNIT(This())))
		{
			workPlace->UseI(C_OBJECT_UNIT(This()));
			SetWorkPlace(workPlace);
			patience++;
			return true;
		}
	}

	patience--;
	return false;

}

void ObjectUnit::ResetHome()
{
	if (!home.expired())
	{
		home.lock()->UnuseR(GetKey());
	}
	home.reset();
	hasHome = false;
}

void ObjectUnit::ResetWorkPlace()
{
	if (!workPlace.expired())
	{
		workPlace.lock()->UnuseI(GetKey());
	}
	workPlace.reset();
	hasWorkPlace = false;
}

void ObjectUnit::CheckHome()
{
	walkPath = ObjectTile::FindShortPath(home, sceneGame.lock()->GetTileInfo(gridCoord).second);
	if (walkPath.empty())
	{
		ResetHome();
		status = STATUS::HOMELESS;
	}
	else
	{
		startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
		destination = sceneGame.lock()->GetTileInfo(walkPath.front()).second;
		status = STATUS::TO_HOME;
	}
}

void ObjectUnit::CheckWorkPlace()
{
	walkPath = ObjectTile::FindShortPath(sceneGame.lock()->GetTileInfo(gridCoord).second, workPlace);
	if (walkPath.empty())
	{
		ResetWorkPlace();
		status = STATUS::HOMELESS;
	}
	else
	{
		startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
		destination = sceneGame.lock()->GetTileInfo(walkPath.back()).second;
		status = STATUS::TO_WORK_PLACE;
	}
}

void ObjectUnit::LifeCycle(float timeDelta, float timeScale)
{
	sf::Vector2f gridSize = sceneGame.lock()->GetGridSize();
	switch (status)
	{
	case STATUS::HOMELESS:
		if (hasHome)
		{
			CheckHome();
		}
		else if (hasWorkPlace)
		{
			CheckWorkPlace();
		}
		else
		{
		}
	case STATUS::HOME:
		if (!hasHome)
		{
			if (hasWorkPlace)
			{
				CheckWorkPlace();
			}
			else
			{
				status = STATUS::HOMELESS;
			}
		}
		else if (hasWorkPlace)
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
					startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
					destination = sceneGame.lock()->GetTileInfo(walkPath.back()).second;
					status = STATUS::TO_WORK_PLACE;
				}
			}
		}
		break;
	case STATUS::TO_WORK_PLACE:
		if (hasWorkPlace)
		{
			if (destination.expired())
			{
				ResetWorkPlace();
				CheckHome();
			}
			Moving(timeDelta, timeScale);
			if (!isMoving)
			{
				if (position == destination.lock()->GetGridCenterPos())
				{
					startingPoint.reset();
					destination.reset();
					status = STATUS::WORK_PLACE;
				}
			}
		}
		else if (hasHome)
		{
			CheckHome();
		}
		else
		{
			status = STATUS::HOMELESS;
		}
		break;
	case STATUS::WORK_PLACE:
		if (!hasWorkPlace)
		{
			if (hasHome)
			{
				CheckHome();
			}
			else
			{
				status = STATUS::HOMELESS;
			}
		}
		else if (hasHome)
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
					startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
					destination = sceneGame.lock()->GetTileInfo(walkPath.front()).second;
					status = STATUS::TO_HOME;
				}
			}
		}
		break;
	case STATUS::TO_HOME:
		if (hasHome)
		{
			MovingReverse(timeDelta, timeScale);
			if (!isMoving)
			{
				if (destination.expired())
				{
					ResetHome();
					CheckWorkPlace();
				}
				else if (position == destination.lock()->GetGridCenterPos())
				{
					startingPoint.reset();
					destination.reset();
					status = STATUS::HOME;
				}
			}
		}
		else if (hasWorkPlace)
		{
			CheckWorkPlace();
		}
		else
		{
			status = STATUS::HOMELESS;
		}
		break;
	default:
		break;
	}

}
void ObjectUnit::Moving(float timeDelta, float timeScale)
{
	if (!isMoving)
	{
		if (!walkPath.empty())
		{
			this->nextTile = sceneGame.lock()->GetTileInfo(walkPath.front()).second;
			walkPath.pop_front();
		}
	}

	bool doCheck = false;
	if (nextTile.expired())
		doCheck = true;
	else if (!walkPath.empty() && startingPoint.lock() != nextTile.lock())
	{
		std::list<GAME_OBJECT_TAG> nextTags;
		nextTags = nextTile.lock()->GetGameObjectTagList();
		if (std::find(nextTags.begin(), nextTags.end(), GAME_OBJECT_TAG::MOVEABLE) == nextTags.end())
			doCheck = true;
	}
	if (doCheck)
	{
		startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
		walkPath = ObjectTile::FindShortPath(destination, startingPoint);
		if (walkPath.empty())
		{
			if (status == STATUS::TO_WORK_PLACE)
				ResetWorkPlace();
			isMoving = false;
			return;
		}
		nextTile = sceneGame.lock()->GetTileInfo(walkPath.front()).second;
		walkPath.pop_front();
	}
	isMoving = true;

	sf::Vector2f nextGridCenterPosition = nextTile.lock()->GetGridCenterPos();
	direction = nextGridCenterPosition - position;
	tool::Normalize(direction);
	SetPosition(position + direction * speed * timeDelta * timeScale);

	float arriveRange = tool::Magnitude(direction * speed * timeDelta * timeScale);
	if (tool::Distance(position, nextGridCenterPosition) <= arriveRange)
	{
		SetPosition(nextGridCenterPosition);
		isMoving = false;
	}
}

void ObjectUnit::MovingReverse(float timeDelta, float timeScale)
{
	if (!isMoving)
	{
		if (!walkPath.empty())
		{
			this->nextTile = sceneGame.lock()->GetTileInfo(walkPath.back()).second;
			walkPath.pop_back();
		}
	}


	bool doCheck = false;
	if (nextTile.expired())
		doCheck = true;
	else if (!walkPath.empty() && startingPoint.lock() != nextTile.lock())
	{
		std::list<GAME_OBJECT_TAG> nextTags;
		nextTags = nextTile.lock()->GetGameObjectTagList();
		if (std::find(nextTags.begin(), nextTags.end(), GAME_OBJECT_TAG::MOVEABLE) == nextTags.end())
			doCheck = true;
	}
	if (doCheck)
	{
		startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
		walkPath = ObjectTile::FindShortPath(destination, startingPoint);
		if (walkPath.empty())
		{
			if (status == STATUS::TO_HOME)
				ResetHome();
			isMoving = false;
			return;
		}
		nextTile = sceneGame.lock()->GetTileInfo(walkPath.back()).second;
		walkPath.pop_back();
	}
	isMoving = true;

	sf::Vector2f nextGridCenterPosition = nextTile.lock()->GetGridCenterPos();
	direction = nextGridCenterPosition - position;
	tool::Normalize(direction);
	SetPosition(position + direction * speed * timeDelta * timeScale);

	float arriveRange = tool::Magnitude(direction * speed * timeDelta * timeScale);
	if (tool::Distance(position, nextGridCenterPosition) <= arriveRange)
	{
		SetPosition(nextGridCenterPosition);
		isMoving = false;
	}
}

void ObjectUnit::SetHome(std::shared_ptr<TileBuilding> building)
{
	ResetHome();
	home = building;
	hasHome = true;

	if (!isCitizen)
	{
		BeCitizen();
		SetPosition(home.lock()->GetGridCenterPos());
		status = STATUS::HOME;
		sceneGame.lock()->unitOnGrid[gridCoord.x][gridCoord.y]
			.push_back(C_OBJECT_UNIT(This()));
		preGridCoord = gridCoord;
	}

	if (!workPlace.expired())
	{
		pathToWorkPlace = ObjectTile::FindShortPath(home, workPlace);
	}
}

void ObjectUnit::SetWorkPlace(std::weak_ptr<TileBuilding> building)
{
	ResetWorkPlace();
	workPlace = building;
	hasWorkPlace = true;
}
