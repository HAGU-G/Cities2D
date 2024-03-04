#include "pch.h"
#include "IOManager.h"



std::list<sf::Keyboard::Key> IOManager::keyPressList;
std::list<sf::Keyboard::Key> IOManager::keyDownList;
std::list<sf::Keyboard::Key> IOManager::keyUpList;
std::deque<std::pair<sf::Keyboard::Key, IOManager::KEY_STATE>> IOManager::combo;
float IOManager::comboTimer = 0.f;
float IOManager::comboTimeLimit = 1.f;
bool IOManager::doComboRecord = false;

void IOManager::EventUpdate(const sf::Event& event)
{

	switch (event.type)
	{
	case sf::Event::KeyPressed:
		if (!IsKeyPress(event.key.code))
		{
			keyPressList.push_back(event.key.code);
			keyDownList.push_back(event.key.code);
			if (doComboRecord)
				combo.push_back({ event.key.code, KEY_STATE::DOWN });
		}
		break;
	case sf::Event::KeyReleased:
		keyPressList.remove(event.key.code);
		keyUpList.push_back(event.key.code);
		if (doComboRecord)
			combo.push_back({ event.key.code, KEY_STATE::UP });
		break;
	}
}

void IOManager::Update(float timedetla, float timeScale)
{
	keyDownList.clear();
	keyUpList.clear();
	if (doComboRecord && (comboTimer += timedetla * timeScale) > comboTimeLimit)
	{
		doComboRecord = false;
	}
}

bool IOManager::IsKeyPress(const sf::Keyboard::Key key)
{
	return find(keyPressList.begin(), keyPressList.end(), key) != keyPressList.end();
}

bool IOManager::IsKeyDown(const sf::Keyboard::Key key)
{
	return find(keyDownList.begin(), keyDownList.end(), key) != keyDownList.end();
}
bool IOManager::IsKeyUp(const sf::Keyboard::Key key)
{
	return find(keyUpList.begin(), keyUpList.end(), key) != keyUpList.end();
}
void IOManager::ClearKeyList()
{
	keyPressList.clear();
	keyDownList.clear();
	keyUpList.clear();
}


bool IOManager::IsPerpectCombo(const SFGM_COMBO& combo)
{
	auto c = combo.begin();
	auto input = IOManager::combo.begin();
	while (c != combo.end() && input != IOManager::combo.end())
	{
		if (*c != *input)
			break;
		c++;
		input++;
		if (c == combo.end())
			return true;
	}
	return false;
}
bool IOManager::IsComboSuccess(const SFGM_COMBO& combo)
{
	auto c = combo.begin();
	for (auto& input : IOManager::combo)
	{
		if (c->second == KEY_STATE::UP)
			c++;
		if (c == combo.end())
			return true;
		if (input == *c)
			c++;
		if (c == combo.end())
			return true;
	}
	return false;
}
bool IOManager::IsExllentCombo(const SFGM_COMBO& combo)
{
	auto c = combo.begin();
	for (auto& input : IOManager::combo)
	{
		if (c->second == KEY_STATE::UP)
			c++;
		if (c == combo.end())
			return true;
		if (input.second == KEY_STATE::UP)
			continue;
		if (input == *c)
			c++;
		else
			return false;
		if (c == combo.end())
			return true;
	}
	return false;
}
void IOManager::ComboRecord(float timeLimit)
{
	if (!doComboRecord)
	{
		combo.clear();
		comboTimeLimit = timeLimit;
		comboTimer = 0.f;
		doComboRecord = true;
	}
}
void IOManager::StopComboRecord()
{
	doComboRecord = false;
}
void IOManager::ClearCombo()
{
	combo.clear();
}