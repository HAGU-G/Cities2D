#pragma once
#include "Scene.h"
class ButtonNineSlice;

class SceneMenu : public Scene
{
protected:
	sf::Sound menuBgm;
	sf::RectangleShape background;

	bool doDrawBackground = false;

	std::shared_ptr<ButtonNineSlice> lastGame;
	std::shared_ptr<ButtonNineSlice> playGame;
	std::shared_ptr<ButtonNineSlice> newGame;
	std::shared_ptr<ButtonNineSlice> saveGame;
	std::shared_ptr<ButtonNineSlice> resetGame;

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

	void Continue();
	void GameContinue();
	void New();
	void Option();

	void Save();
	void Load();
	void Reset();

	void UseBackground();
};

