#pragma once
#include "Scene.h"
class SceneTest : public Scene
{
protected:
	sf::Vector2i mousePosGrid;
	void SetMousePosGrid();

	float gridSize = 100.f;


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

	inline const sf::Vector2i& GetMousePosGrid() const { return mousePosGrid; }
	inline const sf::Vector2f& GetSelectGridPos() const { return sf::Vector2f(mousePosGrid) * gridSize; }
	inline float GetGridSize() const { return gridSize; }

};

