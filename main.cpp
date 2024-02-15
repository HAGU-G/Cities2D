#include "pch.h"
#include <iostream>


class testObj : public sfgm::Object
{
public:
	testObj(std::string str)
		:Object(str)
	{
		Init();
	}
	sf::Sprite sprite;

	void Draww(sf::RenderWindow& wn)
	{
		wn.draw(sprite);
	}
protected:
	void Update(float timeDelta, float timeScale) override
	{

	}
	void Draw() override
	{

	}
	void Init() override
	{
		sf::Texture tex;
		tex.loadFromFile("graphics/tree");
		sprite.setTexture(tex);
	}
};

class testScene : public sfgm::Scene
{
protected:
	void Init() override
	{

	}
	void Update(float timeDelta, float timeScale) override
	{

	}
	void Draw() override
	{

	}
};

std::string k;

int main()
{
	sf::RenderWindow window(sf::VideoMode(100, 100), "test", sf::Style::Default);

	testScene dd;
	std::shared_ptr<testObj> a(new testObj("name"));
	//std::shared_ptr<int> a(new int(5));
	dd.AddObject(a);
	k = a->GetKey();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		std::dynamic_pointer_cast<testObj>((dd.GetObjects().find(k)->second[0]))->Draww(window);
		std::cout << std::dynamic_pointer_cast<testObj>((dd.GetObjects().find(k)->second[0]))->GetKey();
		window.display();
	}
}



