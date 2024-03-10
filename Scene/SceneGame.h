#pragma once
#include "Scene.h"
#include "Defines.h"
#include "RCIManager.h"

class ObjectTileMap;

class SceneGame : public Scene
{
protected:
	sf::Vector2i mousePosGrid;
	void SetMousePosGrid();
	sf::Vector2f gridSize = { 50.f, 50.f };

	GridInfo gridInfo; //[x][y]
	std::shared_ptr<ObjectTileMap> groundTileMap;
	std::unordered_map<std::string, std::weak_ptr<ObjectUnit>> unitList;

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
	void Draw(sf::RenderWindow& window) override;
	void Reset() override;
	void Release() override;

	bool CreateObjectTile(GAME_OBJECT_TYPE type, const sf::Vector2i& gridCoord);
	void OrganizeGridInfo();
	std::shared_ptr<ObjectUnit> AddUnit(const std::shared_ptr<ObjectUnit>& unit);

	void DeleteObjectTile(const sf::Vector2i& gridCoord);

	inline const sf::Vector2i& GetMousePosGrid() const { return mousePosGrid; }
	inline sf::Vector2f GetSelectGridPos() const { return sf::Vector2f(mousePosGrid) * gridSize.x; }
	inline const sf::Vector2f& GetGridSize() const { return gridSize; }
	const GridInfo& GetGridInfo();
	GridInfo& GetGridInfoRaw();
	inline const TileInfo& GetTileInfo(int x, int y) { return gridInfo[x][y]; }
	inline const TileInfo& GetTileInfo(sf::Vector2i gridCoord) { 
		gridInfo;
		return gridInfo[gridCoord.x][gridCoord.y]; }
	inline const std::unordered_map<std::string, std::weak_ptr<ObjectUnit>>& GetUnitList() { return unitList; };

	void SaveGame();
	void LoadGame();
	bool LoadObjectTile(GAME_OBJECT_TYPE type, const sf::Vector2i& gridCoord,
		const std::list<GAME_OBJECT_TAG>& tagList, const sf::IntRect& rect, const RCI& rci);

};

