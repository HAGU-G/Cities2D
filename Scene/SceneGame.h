#pragma once
#include "Scene.h"
#include "Defines.h"
#include "RCIManager.h"

class ObjectTileMap;

class SceneGame : public Scene
{
protected:
	std::deque<sf::Vector2i> deleteTileDeque;

	sf::Vector2i mousePosGrid;
	float viewZoom = 0.0f;
	sf::Vector2f viewZoomTarget;
	bool isTilt = false;
	float startTilt = 0.f;
	float tilt = 0.f;

	sf::Vector2f gridSize = { 50.f, 50.f };
	sf::RectangleShape background;

	GridInfo gridInfo; //[x][y]
	std::shared_ptr<ObjectTileMap> groundTileMap;
	std::unordered_map<std::string, std::weak_ptr<ObjectUnit>> unitList;

	float citizenTimer = 0.f;
	float citizenInterval = 1.f;

	void SetMousePosGrid();

	//도시의 정보
	int money = 5000;
	unsigned int moneyProfit = 0;
	unsigned int moneyLoss = 0;
	time_t cityTime;
	int lastDay;
	int lastMonth;
	bool doPayTex = false;
	float cityTimer = 0.f;
	float cityInterval = 1.f;

public:
	UnitOnGrid unitOnGrid; //[x][y]

	explicit SceneGame(const std::string& name);
	~SceneGame() override = default;
	SceneGame(const Scene&) = delete;
	SceneGame(SceneGame&&) = delete;
	SceneGame& operator=(const SceneGame&) = delete;
	SceneGame& operator=(SceneGame&&) = delete;

	void AddResource() override;
	void Init() override;
	void PreUpdate(float timeDelta, float timeScale) override;
	void Update(float timeDelta, float timeScale) override;
	void PostUpdate(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;
	void Reset() override;
	void Release() override;

	bool CreateObjectTile(RCI rci, const sf::Vector2i& gridCoord, GAME_OBJECT_TYPE type);
	std::shared_ptr<ObjectUnit> AddUnit(const std::shared_ptr<ObjectUnit>& unit);

	void UpdateCityTime(float timeDelta, float TimeScale);
	inline const time_t& GetCityTime() const { return cityTime; }
	void MoneyProfit(unsigned int value);
	void MoneyLoss(unsigned int value);
	void MoneyReport();
	bool MoneyUse(unsigned int value);

	void DeleteObjectTile(const sf::Vector2i& gridCoord);

	inline const sf::Vector2i& GetMouseGridCoord() const { return mousePosGrid; }
	inline sf::Vector2f GetSelectGridPos() const { return sf::Vector2f(mousePosGrid) * gridSize.x; }
	inline const sf::Vector2f& GetGridSize() const { return gridSize; }
	const GridInfo& GetGridInfo();
	GridInfo& GetGridInfoRaw();
	inline const TileInfo& GetTileInfo(int x, int y) { return gridInfo[x][y]; }
	inline const TileInfo& GetTileInfo(sf::Vector2i gridCoord) { return gridInfo[gridCoord.x][gridCoord.y]; }
	inline const std::unordered_map<std::string, std::weak_ptr<ObjectUnit>>& GetUnitList() { return unitList; };
	inline const std::weak_ptr<ObjectTileMap> GetTileMap() { return groundTileMap; }
	inline int GetMoney() const { return money; }
	inline bool DoPayTex() const { return doPayTex; }

	void OrganizeGridInfo();
	void SaveGame();
	void LoadGame();
	bool LoadObjectTile(const RCI& rci, const sf::Vector2i& gridCoord,
		const std::list<GAME_OBJECT_TAG>& tagList, const sf::IntRect& rect, GAME_OBJECT_TYPE type);

};

