#pragma once
#include "RenderWindowRBR.h"

class SceneTitle;

class GameManager final
{
	friend SceneTitle;
private:
	static sf::RenderWindow window;
	static sf::RenderWindow debugWindow;
	static sf::VertexArray fpsGraph;

	static WINDOW_MODE currentMode;
	static sf::Vector2u currentSize;
	static sf::Vector2f windowRatio;
	static sf::Vector2f defaultSize;

	static sf::Vector2i mousePosScreen;
	static sf::Vector2i mousePosWindow; // = mousePosView

	static sf::Clock globalClock;
	static float globalTimeDelta;
	static float globalTimeScale;
	static float globalTimer;

	static std::random_device* rd; // randomDevice
	static std::mt19937* rg; //randomGenerator


public:
	static std::string lastGameName;

	static void Init();
	static void MainLoop();
	static void Release();

	//Window
	static void DebugUpdate();
	static void Exit();

	//Set
	inline static float SetGlobalTimeScale(float value) { globalTimeScale = value; }
	inline static void SetWindowMode(WINDOW_MODE mode) { currentMode = mode; }
	static void SetWindowSize(unsigned int x);
	static void SetWindowRatio(unsigned int x, unsigned int y);
	static void SetWindowPosition(sf::Vector2i position);

	//Get
	inline static sf::RenderWindow& GetWindow() { return window; }
	inline static sf::Vector2u GetWindowSize() { return currentSize; }
	inline static sf::Vector2f GetWindowRatio() { return windowRatio; }
	inline static sf::Vector2f GetDefaultSize() { return defaultSize; }
	inline static sf::Vector2i GetMousePosScreen() { return sf::Mouse::getPosition(); }
	inline static sf::Vector2i GetMousePosWindow() { return mousePosWindow; }
	inline static float GetGlobalTimeScale() { return globalTimeScale; }
	inline static float GetGlobalTimer() { return globalTimer; }

	//AddScene
	static void AddScene();

	//Random
	inline static unsigned int Random() { return (*rg)(); }
	static float RandomRange(float a, float b);
	static int RandomRange(int a, int b);
};

