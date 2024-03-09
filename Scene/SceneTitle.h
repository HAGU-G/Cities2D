#pragma once
#include "Scene.h"
class SceneTitle :
    public Scene
{
private:
	bool isLoaded = false;
	void LoadingGame();

protected:
	sf::Sprite background00;
	sf::Sprite background01;
	sf::Sprite background02;
	sf::Sound musicGameStart;

	sf::Text textTitle;
	sf::Text textTitle2;
	sf::Text textTitleShadow;

	sf::Sprite loadingIcon;

	float moveTimer = 0.f;
	sf::Vector2f viewCenter;;

	bool firstLoad = true;

public:
	explicit SceneTitle(const std::string& name);
	~SceneTitle() override = default;
	SceneTitle(const Scene&) = delete;
	SceneTitle(SceneTitle&&) = delete;
	SceneTitle& operator=(const SceneTitle&) = delete;
	SceneTitle& operator=(SceneTitle&&) = delete;

	void Init() override;
	void PostInit();
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;

};

