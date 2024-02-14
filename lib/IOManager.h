#pragma once

namespace sfgm {

	class IOManager
	{
	private:
		static std::set<sf::Keyboard::Key> keyDownList;
		static std::set<sf::Keyboard::Key> keyUpList;
		static std::set<sf::Keyboard::Key> keyPressList;

	public:
		static void Update(const sf::Event& event);

		static bool isKeyDown(const sf::Keyboard::Key& key);
		static bool isKeyUp(const sf::Keyboard::Key& key);
		static bool isKeyPress(const sf::Keyboard::Key& key);

		//Update() 전에 사용
		static void ClearList();

	};
}