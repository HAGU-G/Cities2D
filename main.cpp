#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "SFML-GM/GameObject.h"
#include "SFML-GM/Scene.h"
#include "SFML-GM/IOManager.h"
//#include "SFML-GM/Defines.h"

using namespace sfgm;

int main()
{

	sf::RenderWindow window(sf::VideoMode(1000, 1000), "test", sf::Style::Default);
	sf::Clock d;

	//메인 루프
	while (window.isOpen())
	{
		float dt = d.restart().asSeconds();

		//이벤트 루프
		IOManager::Update(dt, 1.f);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			IOManager::EventUpdate(event);
		}

		//업데이트

		window.clear();
		//드로우

		window.display();
	}
}



