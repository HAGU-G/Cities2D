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
	SFGM_RCI.UseRegidence(-1);
}

void ObjectUnit::NoWorkPlace()
{
	workPlace.reset();
	hasWorkPlace = false;
	SFGM_RCI.UseIndustry(-1);
}

void ObjectUnit::NoShop()
{
	shop.reset();
	SFGM_RCI.UseCommerce(-1);
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
	if (isCitizen)
	{
		GridUpdate();
	}

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
	ShopUsed();

	NoCitizen();
	status = STATUS::NONE;

	pathToWorkPlace.clear();
	walkPath.clear();
	isMoving = false;

	money = 100;
	findTimer = 0.f;
	patience = 10;

	lifeTimer = 0.f;
	lifeInterval = 1.f;

	speed = 50.f;

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
	ShopUsed();
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
}

void ObjectUnit::GridUpdate()
{
	std::shared_ptr<SceneGame> sceneGame = this->sceneGame.lock();
	if (preGridCoord != gridCoord)
	{
		const std::shared_ptr<ObjectUnit>& thisPtr = C_OBJECT_UNIT(This());

		sceneGame->unitOnGrid[preGridCoord.x][preGridCoord.y]
			.remove_if(
				[thisPtr](std::weak_ptr<ObjectUnit> ptr)
				{
					return thisPtr.get() == ptr.lock().get();
				});

		sceneGame->unitOnGrid[gridCoord.x][gridCoord.y]
			.push_back(thisPtr);
		preGridCoord = gridCoord;
	}
	else if (sceneGame->GetTileInfo(gridCoord).first == GAME_OBJECT_TYPE::NONE)
	{
		Reset();
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
			NoHome();
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
			NoWorkPlace();
		}
	}
}

void ObjectUnit::UpdateShop(float timeDelta, float timeScale)
{
	if (shop.expired()) //상점 파괴됨
	{
		NoShop();
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

	patience--;
	return false;
}

bool ObjectUnit::FindWorkSpace()
{
	money -= 10;
	if (money < 0)
	{
		patience--;
	}
	pathToWorkPlace = ObjectTile::FindShortPath(home, GAME_OBJECT_TAG::I);

	if (pathToWorkPlace.empty())
	{

		return false;
	}
	else
	{
		std::shared_ptr<TileBuilding> workPlace = C_TILE_BUILDING(sceneGame.lock()->GetTileInfo(pathToWorkPlace.back()).second.lock());
		if (workPlace->CanUseI(C_OBJECT_UNIT(This())))
		{
			workPlace->UseI(C_OBJECT_UNIT(This()));
			SetWorkPlace(workPlace);

			return true;
		}
	}


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

void ObjectUnit::ShopUsed()
{
	needShopTimer = 0.f;

	if (needShop)
	{
		needShop = false;
		SFGM_RCI.INeedCommerce(-1);
		patience++;
	}
}

void ObjectUnit::LifeCycle(float timeDelta, float timeScale)
{
	needShopTimer += timeDelta * timeScale;
	if (needShopTimer >= needShopInterval)
	{
		needShopTimer = 0.f;
		if (!needShop)
		{
			needShop = true;
			SFGM_RCI.INeedCommerce(1);
		}
		else
		{
			patience--;
		}
	}
	if (sceneGame.lock()->DoPayTex())
	{
		sceneGame.lock()->MoneyTex(money * 0.10);
		money -= money * 0.10;
		if (!home.expired())
			money += home.lock()->GetRCI().tex / 2;
		if (money < 0)
		{
			home.lock()->UnuseR(GetKey());
			hasHome = false;
		}
	}

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
		needShopTimer += timeDelta * timeScale;
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
				money += 18;
				sceneGame.lock()->MoneyProfit(2);
				if (home.lock() == workPlace.lock())
				{
					status = STATUS::HOME;
				}
				else if (needShop)
				{
					walkPath = ObjectTile::FindShortPath(workPlace, GAME_OBJECT_TAG::C);
					if (walkPath.empty())
					{
						walkPath = pathToWorkPlace;
						startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
						destination = sceneGame.lock()->GetTileInfo(walkPath.front()).second;
						status = STATUS::TO_HOME;
					}
					else
					{
						startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
						destination = sceneGame.lock()->GetTileInfo(walkPath.back()).second;
						shop = C_TILE_BUILDING(destination.lock());
						shop.lock()->UseC(C_OBJECT_UNIT(This()));
						ShopUsed();
						status = STATUS::TO_SHOP;
					}

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
	case STATUS::SHOP:
		needShopTimer -= timeDelta * timeScale;
		if (shop.expired())
		{
			if (hasHome)
			{
				walkPath = pathToWorkPlace;
				startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
				destination = sceneGame.lock()->GetTileInfo(walkPath.front()).second;
				status = STATUS::TO_HOME;
			}
			else if (hasWorkPlace)
			{
				walkPath = pathToWorkPlace;
				startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
				destination = sceneGame.lock()->GetTileInfo(walkPath.back()).second;
				status = STATUS::TO_WORK_PLACE;
			}
			else
			{
				status = STATUS::HOMELESS;
			}
		}
		else
		{
			if (lifeTimer < lifeInterval)
			{
				lifeTimer += timeDelta * timeScale;
			}
			else
			{
				lifeTimer = 0.f;
				shop.lock()->UnuseC(GetKey());
				shop.reset();
				ShopUsed();
				money -= 10;
				sceneGame.lock()->MoneyProfit(1);
				if (home.lock() == workPlace.lock())
				{
					status = STATUS::HOME;
				}
				else if (hasHome)
				{
					walkPath = ObjectTile::FindShortPath(home, sceneGame.lock()->GetTileInfo(gridCoord).second);
					startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
					destination = sceneGame.lock()->GetTileInfo(walkPath.front()).second;
					money -= 2;
					status = STATUS::TO_HOME;
				}
				else if (hasWorkPlace)
				{
					walkPath = ObjectTile::FindShortPath(sceneGame.lock()->GetTileInfo(gridCoord).second, workPlace);
					startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
					destination = sceneGame.lock()->GetTileInfo(walkPath.back()).second;
					money -= 6;
					status = STATUS::TO_WORK_PLACE;
				}
				else
				{
					status = STATUS::HOMELESS;
				}
			}
		}
		break;
	case STATUS::TO_SHOP:
		if (shop.expired())
		{
			if (hasHome)
			{
				walkPath = ObjectTile::FindShortPath(home, sceneGame.lock()->GetTileInfo(gridCoord).second);
				startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
				destination = sceneGame.lock()->GetTileInfo(walkPath.front()).second;
				money -= 10;
				sceneGame.lock()->MoneyProfit(1);
				status = STATUS::TO_HOME;
			}
			else if (hasWorkPlace)
			{
				walkPath = ObjectTile::FindShortPath(sceneGame.lock()->GetTileInfo(gridCoord).second, workPlace);
				startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
				destination = sceneGame.lock()->GetTileInfo(walkPath.back()).second;
				status = STATUS::TO_WORK_PLACE;
			}
			else
			{
				status = STATUS::HOMELESS;
			}
		}
		else
		{
			Moving(timeDelta, timeScale);
			if (!isMoving && position == destination.lock()->GetGridCenterPos())
			{
				money -= 1;
				startingPoint.reset();
				destination.reset();
				status = STATUS::SHOP;
			}
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
