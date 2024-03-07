#pragma once
#ifndef IOMANAGER_H
#define IOMANAGER_H






static class IOManager final
{
public:
	enum class KEY_STATE
	{
		PRESS,
		DOWN,
		UP
	};

	typedef std::deque<std::pair<sf::Keyboard::Key, KEY_STATE>> SFGM_COMBO;

private:
	static std::list<sf::Keyboard::Key> keyPressList;
	static std::list<sf::Keyboard::Key> keyDownList;
	static std::list<sf::Keyboard::Key> keyUpList;

	static SFGM_COMBO combo;
	static float comboTimer;
	static float comboTimeLimit;
	static bool doComboRecord;


	//TODO 키설정 파일(txt?)에서 받아오자!

public:
	//메시지 루프 전에 호출
	static void Update(float timedetla, float timeScale);
	//메시지 루프 안에서 호출
	static void EventUpdate(const sf::Event& event);

	//키보드
	static bool IsKeyPress(const sf::Keyboard::Key key);
	static bool IsKeyDown(const sf::Keyboard::Key key);
	static bool IsKeyUp(const sf::Keyboard::Key key);
	static void ClearKeyList();

	//마우스
	static sf::Keyboard::Key MouseKeyConversion(const sf::Mouse::Button mouse);
	static sf::Mouse::Button MouseKeyConversion(const sf::Keyboard::Key key);
	static bool IsKeyPress(const sf::Mouse::Button mouse);
	static bool IsKeyDown(const sf::Mouse::Button mouse);
	static bool IsKeyUp(const sf::Mouse::Button mouse);


	//콤보
	static bool IsPerpectCombo(const SFGM_COMBO& combo);
	static bool IsExllentCombo(const SFGM_COMBO& combo);
	static bool IsComboSuccess(const SFGM_COMBO& combo);
	static void ComboRecord(float timeLimit);
	static void StopComboRecord();
	static void ClearCombo();
};



#endif // !IOMANAGER_H