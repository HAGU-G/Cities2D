#pragma once
#include "GameObject.h"
#include "Animator.h"

class TileBuilding;
class SceneGame;
class DataManager;

class ObjectUnit : public GameObject
{
private:
	friend TileBuilding;
	friend DataManager;
	
	static size_t citizenCount;

public:
	enum class STATUS
	{
		NONE,

		HOME,
		WORK_PLACE,
		SHOP,
		READY,

		WALK,
		MISSING,
		HOMELESS
	};


protected:
	std::weak_ptr<SceneGame> sceneGame;

	sf::Sprite unitSprite;
	Animator spriteAnimator;
	int spriteNum = 0;
	float soundTimer = 0.f;
	float soundDuration = 1.f;
	bool isReset = false;

	sf::Vector2i preGridCoord;
	sf::Vector2i gridCoord;
	sf::Vector2f gridCenterPos;

	std::weak_ptr<TileBuilding> home;
	std::weak_ptr<TileBuilding> workPlace;
	std::weak_ptr<TileBuilding> shop;
	std::deque<sf::Vector2i> pathToWorkPlace;

	bool isCitizen = false;

	float findTimer = 0.f;
	float findInterval = 10.f;
	int	patience = 10;
	int	maxPatience = 10;

	STATUS preStatus = STATUS::NONE;
	STATUS status = STATUS::NONE;
	STATUS nextstatus = STATUS::NONE;
	float lifeTimer = 0.f;
	float lifeInterval = 1.f;
	float needShopTimer = 0.f;
	float needShopInterval = 20.f;
	bool needShop = false;
	int money = 100;

	float speed = 50.f;
	bool isMoving = false;
	std::deque<sf::Vector2i> walkPath;
	std::weak_ptr<ObjectTile> nextTile;
	std::weak_ptr<ObjectTile> destination;

	void BeCitizen();
	void NoCitizen();

	void FindHome();
	void FindWorkPlace();
	void SetHome(std::weak_ptr<TileBuilding> building);
	void SetWorkPlace(std::weak_ptr<TileBuilding> building);
	void BeHomeless();
	void NoHomeLess();
	void LostHome();
	void LostWorkPlace();
	void LeaveShop();

	void DeleteHome();
	void DeleteWorkPlace();
	void DeleteShop();

	void LifeCycle(float timeDelta, float timeScale);
	void SetShop(std::weak_ptr<TileBuilding> building);
	void ShopUsed();

	void GridUpdate();
	void Moving(float timeDelta, float timeScale);
	void ReRoute();

	bool UseMoney(int value);

	void GoHome();
	void GoWorkPlace();


public:
	explicit ObjectUnit(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType);
	~ObjectUnit() override;
	ObjectUnit(const ObjectUnit&) = delete;
	ObjectUnit(ObjectUnit&&) = delete;
	ObjectUnit& operator=(const ObjectUnit&) = delete;
	ObjectUnit& operator=(ObjectUnit&&) = delete;

	void Init();
	void PreUpdate(float timeDelta, float timeScale) override;
	void Update(float timeDelta, float timeScale) override;
	void PostUpdate(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;
	void Reset() override;
	void Release() override;

	static std::shared_ptr<ObjectUnit> Create(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE type);
	void SetPosition(const sf::Vector2f& position) override;
	void SetTexture(int spriteNum);

	inline std::weak_ptr<ObjectTile> GetNextTile() const { return nextTile; }
	inline static size_t GetUnitCount() { return citizenCount; }

	void AutoDrawDeep();

};

