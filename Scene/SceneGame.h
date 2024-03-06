#pragma once
#include "Scene.h"
#include "Defines.h"

class ObjectTileMap;

class SceneGame : public Scene
{
protected:
	sf::Vector2i mousePosGrid;
	void SetMousePosGrid();
	sf::Vector2f gridSize = { 50.f, 50.f };

	GridInfo gridInfo; //[x][y]
	std::shared_ptr<ObjectTileMap> groundTileMap;

public:
	explicit SceneGame(const std::string& name);
	~SceneGame() override = default;
	SceneGame(const Scene&) = delete;
	SceneGame(SceneGame&&) = delete;
	SceneGame& operator=(const SceneGame&) = delete;
	SceneGame& operator=(SceneGame&&) = delete;

	void Init() override;
	void PreUpdate(float timeDelta, float timeScale) override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;

	bool CreateObjectTile(GAME_OBJECT_TYPE type, const sf::Vector2i& gridCoord);
	//정보 확인을 위해 생성된 nullptr를 제거
	void OrganizeGridInfo();

	bool DeleteObjectTile(const sf::Vector2i& gridCoord);

	inline const sf::Vector2i& GetMousePosGrid() const { return mousePosGrid; }
	inline const sf::Vector2f& GetSelectGridPos() const { return sf::Vector2f(mousePosGrid) * gridSize.x; }
	inline const sf::Vector2f& GetGridSize() const { return gridSize; }
	inline const GridInfo& GetGridInfo() const { return gridInfo; }
	inline const TileInfo& GetTileInfo(int x, int y) { return gridInfo[x][y]; }

};

