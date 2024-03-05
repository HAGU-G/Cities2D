#pragma once
#include "Scene.h"

class ObjectTile;

class SceneTest : public Scene
{
protected:
	sf::Vector2i mousePosGrid;
	void SetMousePosGrid();
	float gridSize = 100.f;
	std::unordered_map<int, std::unordered_map<int, GAME_OBJECT_TYPE>> gridInfo; //gridInfo[x][y]

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

	/// <summary>
	/// Ÿ�� ����
	/// </summary>
	/// <param name="gridCoord">: int��</param>
	/// <param name="type">: ���������� NONE�� �˻��Ͽ� false�� return ������, ������.</param>
	/// <returns>���� : true</returns>
	bool CreateObjectTile(const sf::Vector2i& gridCoord, GAME_OBJECT_TYPE type);

	bool DeleteObject(const std::string& key) override;

	inline const sf::Vector2i& GetMousePosGrid() const { return mousePosGrid; }
	inline const sf::Vector2f& GetSelectGridPos() const { return sf::Vector2f(mousePosGrid) * gridSize; }
	inline float GetGridSize() const { return gridSize; }
	void TileUpdate();

};

