#pragma once
#include "GameObject.h"

class TileBuilding;
class SceneGame;

class ObjectUnit : public GameObject
{
	friend TileBuilding;
public:
	enum class STATUS
	{
		NONE,
		HOME,
		WORK_PLACE,
		WALK,
		TO_HOME = WALK,
		TO_WORK_PLACE,
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
	std::deque<sf::Vector2i> pathToWorkPlace;

	bool isCitizen = false;
	bool hasHome = false;
	bool hasWorkPlace = false;

	float findTimer = 0.f;
	float findInterval = 10.f;
	int	patience = 100;

	STATUS status = STATUS::NONE;
	float lifeTimer = 0.f;
	float lifeInterval = 1.f;
	float speed = 50.f;
	bool isMoving = false;
	std::deque<sf::Vector2i> walkPath;
	std::weak_ptr<ObjectTile> nextTile;
	std::weak_ptr<ObjectTile> startingPoint;
	std::weak_ptr<ObjectTile> destination;


	void SetHome(std::weak_ptr<TileBuilding> building);
	void SetWorkPlace(std::weak_ptr<TileBuilding> building);

public:
	explicit ObjectUnit(std::weak_ptr<Scene> scene, GAME_OBJECT_TYPE objectType);
	~ObjectUnit() override;
	ObjectUnit(const ObjectUnit&) = delete;
	ObjectUnit(ObjectUnit&&) = delete;
	ObjectUnit& operator=(const ObjectUnit&) = delete;
	ObjectUnit& operator=(ObjectUnit&&) = delete;

	void Init();
	void Update(float timeDelta, float timeScale) override;
	void PostUpdate(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;
	void Reset() override;
	void Release() override;

	static std::shared_ptr<ObjectUnit> Create(std::weak_ptr<Scene> scene);
	void SetPosition(const sf::Vector2f& position) override;

	inline std::weak_ptr<ObjectTile> GetNextTile() const { return nextTile; }

	void UpdateHome(float timeDelta, float timeScale);
	void UpdateWorkSpace(float timeDelta, float timeScale);
	bool FindHome();
	bool FindWorkSpace();
	void ResetHome();
	void ResetWorkPlace();
	void CheckHome();
	void CheckWorkPlace();

	void GridUpdate();

	void LifeCycle(float timeDelta, float timeScale);
	void Moving(float timeDelta, float timeScale);
	void MovingReverse(float timeDelta, float timeScale);
};

