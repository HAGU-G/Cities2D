#include "pch.h"
#include <iostream>


class testObj : public sfgm::Object
{
public:
	sf::Sprite sprite;
	sf::Texture tex;

	testObj(sfgm::OBJECT_TYPE type)
		:Object(type)
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

class testScene : public sfgm::Scene
{
public:
	void Init() override
	{

	}
	void Update(float timeDelta, float timeScale) override
	{

	}
	void Draw(sf::RenderWindow& window) override
	{
		Scene::Draw(window);
	}
};

std::string k;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "test", sf::Style::Default);

	testScene dd;
	std::shared_ptr<testObj> a(new testObj(sfgm::OBJECT_TYPE::BUILDING));
	//std::shared_ptr<int> a(new int(5));
	dd.AddObject(a);
	k = a->GetKey();
	dd.Init();
	testObj nnn(sfgm::OBJECT_TYPE::BUILDING);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		dd.Draw(window);
		window.display();
	}
}



