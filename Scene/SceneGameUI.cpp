#include "pch.h"
#include "SceneGameUI.h"

SceneGameUI::SceneGameUI(const std::string& name)
	:Scene(name)
{
}

void SceneGameUI::Init()
{
	Scene::Init();
	tempText.setFillColor(sf::Color::White);
	tempText.setCharacterSize(60);
	tempText.setFont(SFGM_FONT.Get(""));
}

void SceneGameUI::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.draw(tempText);
}

void SceneGameUI::UpdateRCIGraph(int r, int c, int i)
{
	tempText.setString(std::to_string(r) + " " + std::to_string(c) + " " + std::to_string(i));
}
