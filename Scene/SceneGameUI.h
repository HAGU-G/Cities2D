#pragma once
#include "Scene.h"
class SceneGameUI : public Scene
{
protected:
	sf::Text tempText;

public:
	explicit SceneGameUI(const std::string& name);
	~SceneGameUI() override = default;
	SceneGameUI(const Scene&) = delete;
	SceneGameUI(SceneGameUI&&) = delete;
	SceneGameUI& operator=(const SceneGameUI&) = delete;
	SceneGameUI& operator=(SceneGameUI&&) = delete;

	void Init() override;
	void Draw(sf::RenderWindow& window) override;

	void UpdateRCIGraph(int r, int c, int i);
};

