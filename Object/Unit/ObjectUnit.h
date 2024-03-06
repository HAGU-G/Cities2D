#pragma once
#include "GameObject.h"

class TileBuilding;
class SceneGame;

class ObjectUnit : public GameObject
{
	friend TileBuilding;
protected:
	std::weak_ptr<SceneGame> sceneGame;

	std::weak_ptr<TileBuilding> home;
	std::weak_ptr<TileBuilding> workPlace;

	bool hasHome = false;
	bool hasworkPlace = false;

	float findTimer = 0.f;
	float findInterval = 10.f;
	int	patience = 5;

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
	void Reset() override;
	void Release() override;

	static std::shared_ptr<ObjectUnit> Create(std::weak_ptr<Scene> scene);
	bool FindHome();
};

