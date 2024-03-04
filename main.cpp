#include "pch.h"
#include "SceneTest.h"

//#include "SFML-GM/Defines.h"

int main()
{

	RenderWindow window(sf::VideoMode(1000, 1000), "test", sf::Style::Default);
	Clock d;
	SceneTest sceneTest("Test");
	sceneTest.Init();

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
		sceneTest.Update(dt,1.f);

		//드로우
		window.clear();
		sceneTest.Draw(window);
		window.display();
	}
}



