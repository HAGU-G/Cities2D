#pragma once
#include "Scene.h"

class ObjectTile;

class SceneTest : public Scene
{
protected:
	sf::Vector2i mousePosGrid;
	void SetMousePosGrid();
	sf::Vector2f gridSize = { 50.f, 50.f };
	std::unordered_map<int, std::unordered_map<int, std::pair<GAME_OBJECT_TYPE,std::shared_ptr<ObjectTile>>>> gridInfo; //gridInfo[x][y]

	sf::RectangleShape test;

public:
	explicit SceneTest(const std::string& name);
	~SceneTest();
	SceneTest(const Scene&) = delete;
	SceneTest(SceneTest&&) = delete;
	SceneTest& operator=(const SceneTest&) = delete;
	SceneTest& operator=(SceneTest&&) = delete;

	void Init() override;
	void PreUpdate(float timeDelta, float timeScale) override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;

	//TODO 타일 정보와 그래프 구조를 관리할 클래스를 따로 지정할 예정. 이 함수는 삭제될 것임.
	bool CreateObjectTile(GAME_OBJECT_TYPE type, const sf::Vector2i& gridCoord);

	bool DeleteObject(const std::string& key) override;
	void DeleteObject(const sf::Vector2i& gridCoord);

	inline const sf::Vector2i& GetMousePosGrid() const { return mousePosGrid; }
	inline const sf::Vector2f& GetSelectGridPos() const { return sf::Vector2f(mousePosGrid) * gridSize.x; }
	inline const sf::Vector2f& GetGridSize() const { return gridSize; }
	void TileUpdate();

};

