#pragma once
#include "Scene.h"
class SceneMenu : public Scene
{
protected:
public:
	explicit SceneMenu(const std::string& name);
	~SceneMenu() override = default;
	SceneMenu(const Scene&) = delete;
	SceneMenu(SceneMenu&&) = delete;
	SceneMenu& operator=(const SceneMenu&) = delete;
	SceneMenu& operator=(SceneMenu&&) = delete;

	void AddResource() override;
	void Init() override;
	void Draw(sf::RenderWindow& window) override;
};

