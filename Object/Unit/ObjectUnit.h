#pragma once
#include "GameObject.h"

class TileBuilding;
class SceneGame;
class DataManager;

class ObjectUnit : public GameObject
{
private:
	friend TileBuilding;
	friend DataManager;
	
	static size_t citizenCount;

	void NoHome();
	void NoWorkPlace();
	void NoShop();
public:
	enum class STATUS
	{
		NONE,

		HOME,
		WORK_PLACE,
		SHOP,

		WALK,
		TO_HOME = WALK,
		TO_WORK_PLACE,
		TO_SHOP,
		HOMELESS,
		WALK_END
	};
protected:
	std::weak_ptr<SceneGame> sceneGame;

	sf::CircleShape tempRender;

	sf::Vector2i preGridCoord;
	sf::Vector2i gridCoord;
	sf::Vector2f gridCenterPos;

	std::weak_ptr<TileBuilding> home;
	std::weak_ptr<TileBuilding> workPlace;
	std::weak_ptr<TileBuilding> shop;
	std::deque<sf::Vector2i> pathToWorkPlace;

	bool isCitizen = false;
	bool hasHome = false;
	bool hasWorkPlace = false;

	float findTimer = 0.f;
	float findInterval = 10.f;
	int	patience = 10;

	STATUS status = STATUS::NONE;
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
	std::weak_ptr<ObjectTile> startingPoint;
	std::weak_ptr<ObjectTile> destination;

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

	static std::shared_ptr<ObjectUnit> Create(std::weak_ptr<Scene> scene);
	void SetPosition(const sf::Vector2f& position) override;

	inline std::weak_ptr<ObjectTile> GetNextTile() const { return nextTile; }
	inline static size_t GetUnitCount() { return citizenCount; }

	void BeCitizen();
	void NoCitizen();

	void UpdateHome(float timeDelta, float timeScale);
	void UpdateWorkSpace(float timeDelta, float timeScale);
	void UpdateShop(float timeDelta, float timeScale);
	bool FindHome();
	void SetHome(std::shared_ptr<TileBuilding> building);
	void ResetHome();
	bool FindWorkSpace();
	void SetWorkPlace(std::weak_ptr<TileBuilding> building);
	void ResetWorkPlace();
	void CheckHome();
	void CheckWorkPlace();
	void ShopUsed();

	void GridUpdate();

	void LifeCycle(float timeDelta, float timeScale);
	void Moving(float timeDelta, float timeScale);
	void MovingReverse(float timeDelta, float timeScale);
};

