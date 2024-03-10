#pragma once
#include "Scene.h"
#include "RCIManager.h"
class ObjectButton;
class ButtonNameTag;
class SceneGame;

class SceneGameUI : public Scene
{
protected:
	std::weak_ptr<SceneGame> sceneGame;

	sf::Text tempText;

	sf::Text textNewGame;
	sf::Text textOption;
	sf::Text textExit;

	sf::RectangleShape underBarBack;
	sf::RectangleShape underBarFront;

	std::shared_ptr<ObjectButton> buttonPlay;
	std::shared_ptr<ObjectButton> buttonPause;
	std::shared_ptr<ObjectButton> buttonRoad;
	std::shared_ptr<ObjectButton> buttonR;
	std::shared_ptr<ObjectButton> buttonC;
	std::shared_ptr<ObjectButton> buttonI;
	std::shared_ptr<ObjectButton> buttonDestroy;
	std::shared_ptr<ButtonNameTag> buttonRCI;
	std::shared_ptr<ButtonNameTag> buttonGrid;
	std::shared_ptr<ButtonNameTag> buttonCitizen;
	std::shared_ptr<ButtonNameTag> buttonMoney;


	RCI rci;
	GAME_OBJECT_TYPE type;
	int clickMode = 0; //0:Nothing 1:build -1:destroy
	sf::Vector2i selectTile;

public:
	explicit SceneGameUI(const std::string& name);
	~SceneGameUI() override = default;
	SceneGameUI(const Scene&) = delete;
	SceneGameUI(SceneGameUI&&) = delete;
	SceneGameUI& operator=(const SceneGameUI&) = delete;
	SceneGameUI& operator=(SceneGameUI&&) = delete;

	void AddResource() override;
	void Init() override;
	void PreUpdate(float timeDelta, float timeScale) override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;

	void UpdateRCIGraph(int r, int c, int i);
	void SetTempText(const std::string& str);

	void Play();
	void Pause();
	void Road();
	void R();
	void C();
	void I();
	void Destroy();
	void Menu();

	void UnSeleteAll();
};

