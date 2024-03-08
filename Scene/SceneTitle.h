#pragma once
#include "Scene.h"
class SceneTitle :
    public Scene
{
protected:
	sf::Sprite background00;
	sf::Sprite background01;
	sf::Sprite background02;
	sf::Music musicGameStart;

	sf::Text textTitle;
	sf::Text textTitle2;
	sf::Text textTitleShadow;

	float moveTimer = 0.f;
	sf::Vector2f viewCenter;;

public:
	explicit SceneTitle(const std::string& name);
	~SceneTitle() override = default;
	SceneTitle(const Scene&) = delete;
	SceneTitle(SceneTitle&&) = delete;
	SceneTitle& operator=(const SceneTitle&) = delete;
	SceneTitle& operator=(SceneTitle&&) = delete;

	void Resource() override;
	void Init() override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;
};

