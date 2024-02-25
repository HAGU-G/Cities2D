#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "SFML-GM/GameObject.h"
#include "SFML-GM/Scene.h"
#include "SFML-GM/IOManager.h"
//#include "SFML-GM/Defines.h"

using namespace sfgm;

class testObj : public GameObject
{
public:
	sf::Sprite sprite;
	sf::Texture tex;

	testObj(GAME_OBJECT_TYPE type)
		:GameObject(type)
	{
		Init();
	}

	void Update(float timeDelta, float timeScale) override
	{

	}
	void Draw(sf::RenderWindow& window) override
	{
		window.draw(sprite);
	}
	void Init() override
	{
		tex.loadFromFile("graphics/bee.png");
		sprite.setTexture(tex);
	}
};

class TestScene : public Scene
{
public:
	TestScene(const std::string& name)
		:Scene(name)
	{

	}
	~TestScene() override = default;
};

std::string k;

int main()
{

	sf::RenderWindow window(sf::VideoMode(1000, 1000), "test", sf::Style::Default);

	TestScene testScene("name");
	std::shared_ptr<testObj> a(new testObj(GAME_OBJECT_TYPE::BUILDING));
	//std::shared_ptr<int> a(new int(5));
	testScene.AddObject(a);
	k = a->GetKey();
	testScene.Init();
	testObj nnn(GAME_OBJECT_TYPE::BUILDING);

	sf::Clock d;

	while (window.isOpen())
	{
		float dt = d.restart().asSeconds();
		IOManager::Instance().Update(dt, 1.f);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			IOManager::Instance().EventUpdate(event);
		}



		if (IOManager::Instance().IsKeyDown(sf::Keyboard::Key::Escape))
		{
			window.close();
		}
		IOManager::SFGM_COMBO combo;
		combo.push_back({ sf::Keyboard::A,IOManager::KEY_STATE::DOWN });
		combo.push_back({ sf::Keyboard::B,IOManager::KEY_STATE::DOWN });
		combo.push_back({ sf::Keyboard::B,IOManager::KEY_STATE::DOWN });
		if (IOManager::Instance().IsKeyDown(sf::Keyboard::Key::A))
		{
			IOManager::Instance().ComboRecord(3.f);
		}

		if (IOManager::Instance().IsComboSuccess(combo))
		{
			std::cout << "ddd" << std::endl;
			window.close();
		}
		if (IOManager::Instance().IsKeyDown(sf::Keyboard::Key::C))
		{
			IOManager::Instance().ComboRecord(3.f);
		}
		window.clear();
		testScene.Draw(window);
		window.display();
	}
}



