#include "pch.h"
#include "ObjectUnit.h"
#include "TileBuilding.h"
#include "SceneGame.h"
#include "Scene.h"

size_t ObjectUnit::citizenCount = 0;

void ObjectUnit::NoHome()
{
	SFGM_RCI.UseRegidence(-1);

	home.reset();
	hasHome = false;
}

void ObjectUnit::NoWorkPlace()
{
	SFGM_RCI.UseIndustry(-1);

	workPlace.reset();
	hasWorkPlace = false;
}

void ObjectUnit::NoShop()
{
	shop.reset();

	if (status == STATUS::TO_SHOP || status == STATUS::SHOP)
	{
		SFGM_RCI.UseCommerce(-1);
		if (!home.expired())
		{
			walkPath = pathToWorkPlace;
			startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
			destination = home;
			status = STATUS::TO_HOME;
		}
		else if (!workPlace.expired())
		{
			walkPath = pathToWorkPlace;
			startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
			destination = workPlace;
			status = STATUS::TO_WORK_PLACE;
		}
		else
		{
			BeHomeLess();
		}
	}
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
	spriteAnimator.SetTarget(&unitSprite);
}

void ObjectUnit::PreUpdate(float timeDelta, float timeScale)
{

	if (isCitizen)
	{
		GridUpdate();
		if (soundTimer <= soundDuration)
			soundTimer += timeDelta;
	}
}

void ObjectUnit::Update(float timeDelta, float timeScale)
{
	if (home.expired())
		FindHome();
	else if (workPlace.expired())
		FindWorkPlace();

	if (isCitizen)
	{
		spriteAnimator.Update(timeDelta, timeScale);
		//GridUpdate();
		LifeCycle(timeDelta, timeScale);
		if (patience > maxPatience)
			patience = maxPatience;
		unitSprite.setRotation(scene.lock()->GetView().getRotation());
		unitSprite.setScale(1.f + (tool::Angle360(direction, scene.lock()->GetView().getRotation()) > 90.f ? -2.f : 0.f), sceneGame.lock()->GetTilt());
	}

}

void ObjectUnit::PostUpdate(float timeDelta, float timeScale)
{
	if (patience <= 0)
	{
		scene.lock()->DeleteObject(GetKey());
	}
	else if (timeScale != 0.f)
	{
		AutoDrawDeep();
	}
}

void ObjectUnit::Draw(sf::RenderWindow& window)
{
	if (status >= STATUS::WALK && status < STATUS::WALK_END)
	{
		window.draw(unitSprite);
	}
}

void ObjectUnit::Reset()
{
	isReset = true;

	ResetHome();
	ResetWorkPlace();
	ShopUsed();
	NoShop();

	NoCitizen();
	status = STATUS::NONE;

	pathToWorkPlace.clear();
	walkPath.clear();
	isMoving = false;

	nextTile.reset();
	startingPoint.reset();
	destination.reset();

	//CitizenData
	const rapidcsv::Document& citizenData = SFGM_CSVFILE.Get("data/CitizenData.csv").GetDocument();
	auto cRow = citizenData.GetRow<float>(0);
	money = GameManager::RandomRange((int)cRow[0], (int)cRow[1]);
	patience = GameManager::RandomRange((int)cRow[2], (int)cRow[3]);
	maxPatience = patience;

	findTimer = 0.f;
	findInterval = GameManager::RandomRange(cRow[4], cRow[5]);

	lifeTimer = 0.f;
	lifeInterval = GameManager::RandomRange(cRow[6], cRow[7]);

	needShop = false;
	needShopTimer = 0.f;
	needShopInterval = GameManager::RandomRange(cRow[8], cRow[9]);
	speed = GameManager::RandomRange(cRow[10], cRow[11]);

	//sprite
	const rapidcsv::Document& unitData = SFGM_CSVFILE.Get("data/UnitData.csv").GetDocument();
	auto uRow = unitData.GetRow<std::string>(0);
	spriteNum = GameManager::RandomRange(0, std::stoi(uRow[4]) - 1);
	SetTexture(spriteNum);

	isReset = false;
}

void ObjectUnit::Release()
{
	isReset = true;
	ResetHome();
	ResetWorkPlace();
	ShopUsed();
	NoShop();

	NoCitizen();
	sceneGame.reset();
	nextTile.reset();
	startingPoint.reset();
	destination.reset();

	GameObject::Release();
}

std::shared_ptr<ObjectUnit> ObjectUnit::Create(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE type)
{
	std::shared_ptr<ObjectUnit> objectUnit = std::make_shared<ObjectUnit>(scene, type);
	scene.lock()->AddObject(objectUnit);
	objectUnit->Init();
	objectUnit->Reset();
	return objectUnit;
}

void ObjectUnit::SetPosition(const sf::Vector2f& position)
{

	GameObject::SetPosition(position);

	gridCoord = sceneGame.lock()->PosToGridCoord(position);

	unitSprite.setPosition(position);
}

void ObjectUnit::SetTexture(int spriteNum)
{
	this->spriteNum = spriteNum;
	const rapidcsv::Document& citizenData = SFGM_CSVFILE.Get("data/CitizenData.csv").GetDocument();
	auto cRow = citizenData.GetRow<float>(0);
	const rapidcsv::Document& unitData = SFGM_CSVFILE.Get("data/UnitData.csv").GetDocument();
	auto uRow = unitData.GetRow<std::string>(0);
	std::string spriteAct = (speed > (cRow[11] + cRow[10]) / 2) ? "-Run" : "-Walk";
	unitSprite.setTexture(SFGM_TEXTURE.Get(uRow[2] + uRow[3] + to_string(spriteNum) + spriteAct + ".png"));
	unitSprite.setOrigin(std::stof(uRow[5]), std::stof(uRow[6]));

	spriteAnimator.ClearEvent();
	spriteAnimator.Play(uRow[2] + uRow[3] + to_string(spriteNum) + spriteAct + ".csv");
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
				FindWorkPlace();
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

void ObjectUnit::BeHomeLess()
{
	const rapidcsv::Document& unitData = SFGM_CSVFILE.Get("data/UnitData.csv").GetDocument();
	auto uRow = unitData.GetRow<std::string>(0);
	unitSprite.setTexture(SFGM_TEXTURE.Get(uRow[2] + uRow[3] + to_string(spriteNum) + "-Laying.png"));
	unitSprite.setOrigin(std::stof(uRow[5]), std::stof(uRow[6]));

	spriteAnimator.ClearEvent();
	spriteAnimator.Play(uRow[2] + uRow[3] + to_string(spriteNum) + "-Laying.csv");
	status = STATUS::HOMELESS;

	if (!isReset && soundTimer >= soundDuration && !isMute)
	{
		soundTimer = 0.f;
		soundDuration = IOManager::PlaySfx("resource/sfx/homeless.wav", { position.x, position.y, 0.f }, 100, 4);
	}
}

void ObjectUnit::NoHomeLess()
{
	const rapidcsv::Document& citizenData = SFGM_CSVFILE.Get("data/CitizenData.csv").GetDocument();
	auto cRow = citizenData.GetRow<float>(0);
	const rapidcsv::Document& unitData = SFGM_CSVFILE.Get("data/UnitData.csv").GetDocument();
	auto uRow = unitData.GetRow<std::string>(0);
	std::string spriteAct = (speed > (cRow[11] + cRow[10]) / 2) ? "-Run" : "-Walk";
	unitSprite.setTexture(SFGM_TEXTURE.Get(uRow[2] + uRow[3] + to_string(spriteNum) + spriteAct + ".png"));
	unitSprite.setOrigin(std::stof(uRow[5]), std::stof(uRow[6]));

	spriteAnimator.ClearEvent();
	spriteAnimator.Play(uRow[2] + uRow[3] + to_string(spriteNum) + spriteAct + ".csv");
}

bool ObjectUnit::UseMoney(int value)
{
	if (money < value)
		return false;
	else
		money -= value;
	return true;
}

void ObjectUnit::FindHome()
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
					SetHome(home);
					return;
				}
			}
		}
	}
	else
	{
		std::deque<sf::Vector2i> pathToHome = ObjectTile::FindShortPath(sceneGame.lock()->GetTileInfo(gridCoord).second, GAME_OBJECT_TAG::R);

		if (!pathToHome.empty())
		{
			std::shared_ptr<TileBuilding> home = C_TILE_BUILDING(sceneGame.lock()->GetTileInfo(pathToHome.back()).second.lock());
			if (home->CanUseR(C_OBJECT_UNIT(This())))
			{
				patience++;
				SetHome(home);
				return;
			}
		}
	}

	patience--;
}

void ObjectUnit::FindWorkPlace()
{
	if (UseMoney(10))
	{
		patience--;
	}

	std::deque<sf::Vector2i> pathToWorkPlace = ObjectTile::FindShortPath(home, GAME_OBJECT_TAG::I);

	if (!pathToWorkPlace.empty())
	{
		std::shared_ptr<TileBuilding> workPlace = C_TILE_BUILDING(sceneGame.lock()->GetTileInfo(pathToWorkPlace.back()).second.lock());
		if (workPlace->CanUseI(C_OBJECT_UNIT(This())))
		{
			patience++;
			SetWorkPlace(workPlace);
			return;
		}
	}

	patience--;
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
		BeHomeLess();
	}
	else
	{
		startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
		destination = sceneGame.lock()->GetTileInfo(walkPath.front()).second;
		NoHomeLess();
		status = STATUS::TO_HOME;
	}
}

void ObjectUnit::CheckWorkPlace()
{
	walkPath = ObjectTile::FindShortPath(sceneGame.lock()->GetTileInfo(gridCoord).second, workPlace);
	if (walkPath.empty())
	{
		ResetWorkPlace();
		BeHomeLess();
	}
	else
	{
		startingPoint = sceneGame.lock()->GetTileInfo(gridCoord).second;
		destination = sceneGame.lock()->GetTileInfo(walkPath.back()).second;
		NoHomeLess();
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
		if (UseMoney(money * 0.10))
			sceneGame.lock()->MoneyTex(money * 0.10);
		else
			patience--;

		if (!home.expired())
		{
			if (UseMoney(-(home.lock()->GetRCI().tex / 100) + 1))
				sceneGame.lock()->MoneyTex(money * 0.10);
			else
				patience--;
		}
		if (money < 0)
		{
			LostHome();
		}
	}



	switch (status)
	{
	case ObjectUnit::STATUS::HOME:
		lifeTimer += timeDelta * timeScale;
		if (lifeTimer >= lifeInterval)
		{
			lifeTimer = 0.f;
			status = STATUS::READY;
			break;
		}
		break;
	case ObjectUnit::STATUS::WORK_PLACE:
		if (lifeTimer == 0.f)
		{
			int pay = -(workPlace.lock()->GetRCI().tex / 100);
			money += pay * 0.9f + 1;
			sceneGame.lock()->MoneyProfit(pay * 0.1f + 1);
		}
		lifeTimer += timeDelta * timeScale;
		if (lifeTimer >= lifeInterval)
		{
			lifeTimer = 0.f;
			status = STATUS::READY;
			break;
		}
		break;
	case ObjectUnit::STATUS::SHOP:
		needShop = false;
		needShopTimer = 0.f;
		if (lifeTimer == 0.f)
		{
			int pay = -(shop.lock()->GetRCI().tex / 100);
			if (UseMoney(pay + 1))
				patience++;
			sceneGame.lock()->MoneyProfit(pay * 0.1f + 1);
		}
		lifeTimer += timeDelta * timeScale;
		if (lifeTimer >= lifeInterval)
		{
			lifeTimer = 0.f;
			status = STATUS::READY;
			break;
		}
		break;
	case ObjectUnit::STATUS::HOMELESS:
		break;
	case ObjectUnit::STATUS::READY:
		if (needShop && money > -(home.lock()->GetRCI().tex / 100) + 1)
		{
			auto pathToShop = ObjectTile::FindShortPath(sceneGame.lock()->GetTileInfo(gridCoord).second, GAME_OBJECT_TAG::C);
			if (!pathToShop.empty())
			{
				std::shared_ptr<TileBuilding> shop = C_TILE_BUILDING(sceneGame.lock()->GetTileInfo(pathToWorkPlace.back()).second.lock());
				if (shop->CanUseC(C_OBJECT_UNIT(This())))
				{
					needShop = false;
					needShopTimer = 0.f;
					patience++;
					SetShop(shop);
					walkPath = pathToShop;
					status = STATUS::WALK;
					break;
				}
			}
		}
		else if (!workPlace.expired() && preStatus == STATUS::HOME)
		{
			walkPath = pathToWorkPlace;
			status = STATUS::WALK;
			break;
		}
		else if (!home.expired())
		{
			if (preStatus == STATUS::WORK_PLACE)
			{
				walkPath.clear();
				auto it = pathToWorkPlace.rbegin();
				while (it != pathToWorkPlace.rend())
				{
					walkPath.push_back(*it);
					it++;
				}
			}
			else
			{
				walkPath = ObjectTile::FindShortPath(sceneGame.lock()->GetTileInfo(gridCoord).second, home);
			}
			status = STATUS::WALK;
			break;
		}
		break;
	case ObjectUnit::STATUS::WALK:

		//TODO 걸어가는 것 수정
		// 시민인 상태에서 집을 찾는 것 수정
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
		if (soundTimer >= soundDuration && !isMute)
		{
			soundTimer = 0.f;
			soundDuration = IOManager::PlaySfx("resource/sfx/leave.wav", { position.x, position.y, 0.f }, 30, 7);
		}

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

void ObjectUnit::AutoDrawDeep()
{
	sceneGame.lock()->SetDoDrawlayerSort(true);
	const Frustum& frustum = sceneGame.lock()->GetFrustum();
	sf::Vector3f veiwCenter = tool::To3D(sceneGame.lock()->GetView().getCenter());

	sf::Vector3f plane = frustum.nearPlane - veiwCenter;
	sf::Vector3f citizen = tool::To3D(position) - frustum.nearPlane;
	float onN = tool::OnPlane(plane, citizen);
	if (onN > 0.f)
	{
		drawDeep = 0.f;
		isShow = false;
		return;
	}

	plane = frustum.farPlane - veiwCenter;
	citizen = tool::To3D(position) - frustum.farPlane;
	float onF = tool::OnPlane(plane, citizen);
	if (onF > 0.f)
	{
		drawDeep = 0.f;
		isShow = false;
		return;
	}

	plane = frustum.leftPlane - veiwCenter;
	citizen = tool::To3D(position) - frustum.leftPlane;
	float onL = tool::OnPlane(plane, citizen);
	if (onL > 0.f)
	{
		drawDeep = 0.f;
		isShow = false;
		return;
	}

	plane = frustum.rightPlane - veiwCenter;
	citizen = tool::To3D(position) - frustum.rightPlane;
	float onR = tool::OnPlane(plane, citizen);
	if (onR > 0.f)
	{
		drawDeep = 0.f;
		isShow = false;
		return;
	}
	else
	{
		drawDeep = -onN;
		isShow = true;
	}
}

void ObjectUnit::SetHome(std::shared_ptr<TileBuilding> building)
{
	home = building;
	//TODO 집 슬롯에 등록

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
	//TODO 회사 슬롯에 등록
	workPlace = building;
}

void ObjectUnit::LostHome()
{
	//TODO 집 슬롯에서 삭제
	home.reset();
}

void ObjectUnit::SetShop(std::weak_ptr<TileBuilding> building)
{
	//상점 슬롯에 등록
	shop = building;
}
