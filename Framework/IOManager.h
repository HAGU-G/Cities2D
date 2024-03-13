#pragma once
#ifndef IOMANAGER_H
#define IOMANAGER_H

class IOManager final
{

//////////////////////////////////////////////////////////////////////////////////////////////
//    
//     Key
// 
//////////////////////////////////////////////////////////////////////////////////////////////
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

	static bool isMouseInWindow;
	static float wheelDelta;

	static std::wstring inputText;
	static bool doInputText;
	static size_t textSize;

public:
	//메시지 루프 전에 호출
	static void KeyUpdate(float timedetla, float timeScale);
	//메시지 루프 안에서 호출
	static void KeyEventUpdate(const sf::Event& event);

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
	static float GetWheelDelta();
	inline static bool IsMouseInWindow() { return isMouseInWindow; }

	//콤보
	static bool IsPerpectCombo(const SFGM_COMBO& combo);
	static bool IsExllentCombo(const SFGM_COMBO& combo);
	static bool IsComboSuccess(const SFGM_COMBO& combo);
	static void ComboRecord(float timeLimit);
	static void StopComboRecord();
	static void ClearCombo();

	//텍스트 입력
	static void SetDoInputText(bool value, size_t textSize = IOManager::textSize);
	inline static bool IsDoInputText() { return doInputText; };
	inline static void ClearInputText() { inputText.clear(); }
	inline static const std::wstring& GetInputText() { return inputText; }




//////////////////////////////////////////////////////////////////////////////////////////////
//    
//     Sound
// 
//////////////////////////////////////////////////////////////////////////////////////////////

private:
	static std::pair<std::pair<std::string, sf::Sound>, std::pair<std::string, sf::Sound>> bgm[2]; // 0=main

	static std::list<std::shared_ptr<sf::Sound>> usingSfx;
	static std::list<std::shared_ptr<sf::Sound>> waitingSfx;

public:
	static void SoundInit(int sfxCount = 64);
	static void SoundUpdate(float timeDelta, float timeScale);
	static void SoundRelease();

	static void PlayBGMCh1(const std::string& path, bool loop = true);
	static void PlayBGMCh2(const std::string& path, bool loop = true);
	static void StopBGM();
	static void StopBGM(unsigned int channel);
	static void SetBGMCh1Volume(float volume);
	static void SetBGMCh2Volume(float volume);
	static void SetBGMVolume(); //임시

	static void SetVolume(float volume);

	static void BGMSyncPlay(const std::string& path1, const std::string& path2, bool loop = true);
	static void BGMSyncPlay(const std::string& path1, const std::string& path2, unsigned int channel, bool loop);
	static void BGMSyncSwitch(unsigned int channel);


	static std::shared_ptr<sf::Sound> PlaySfx(const std::string& path, bool listener = true, bool play = true);
	static float PlaySfx(const std::string& path, sf::Vector3f pos, float minD, float Atte);

};


#endif // !IOMANAGER_H