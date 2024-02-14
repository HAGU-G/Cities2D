#include <iostream>
#include "pch.h"


class testObj : public sfgm::Object
{
public:
	testObj(std::string str)
		:Object(str)
	{

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



int main()
{
	sf::RenderWindow window(sf::VideoMode(100, 100), "test", sf::Style::Default);

	testScene dd;
	std::shared_ptr<testObj> a(new testObj("name"));
	//std::shared_ptr<int> a(new int(5));
	dd.AddObject(a);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		dd.pre_Init();
		window.clear();
		dd.pre_Update(1.f, 1.f);
		dd.pre_Draw();
		auto& it = dd.GetObjects().find("name")->second;
		for (auto& i : it)
		{
			dynamic_cast<testObj*>(i.get())->Draww(window);
		}
		window.display();
	}
}



