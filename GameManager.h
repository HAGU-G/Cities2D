#pragma once
#include "RenderWindowRBR.h"


class GameManager final
{
private:
	static unsigned int titleY;
	static sf::RenderWindow window;
	static WINDOW_MODE currentMode;
	static sf::Vector2u currentSize;
	static unsigned int ratioX;
	static unsigned int ratioY;

	static sf::Vector2i mousePosScreen;
	static sf::Vector2i mousePosWindow; // = mousePosView

	static sf::Clock globalClock;
	static float globalTimeDelta;
	static float globalTimeScale;
	static float globalTimer;

public:
	static void Init();
	static void MainLoop();
	static void Release();

	//Set
	inline static float SetGlobalTimeScale(float value) { globalTimeScale = value; }
	inline static void SetWindowMode(WINDOW_MODE mode) { currentMode = mode; }
	static void SetWindowSize(unsigned int x);
	static void SetWindowRatio(unsigned int x, unsigned int y);
	static void SetWindowPosition(sf::Vector2i position);

	//Get
	inline static const sf::RenderWindow& GetWindow() { return window; }
	inline static const sf::Vector2u& GetWindowSize() { return currentSize; }
	inline static unsigned int GetTitleY() { return titleY; }
	inline static const sf::Vector2i& GetMousePosScreen() { return sf::Mouse::getPosition(); }
	inline static const sf::Vector2i& GetMousePosWindow() { return mousePosWindow; }
	inline static float GetGlobalTimeScale() { return globalTimeScale; }
	inline static float GetGlobalTimer() { return globalTimer; }

	//AddScene
	static void AddScene();
};

