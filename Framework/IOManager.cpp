#include "pch.h"
#include "IOManager.h"
#include "rapidcsv.h"

std::list<sf::Keyboard::Key> IOManager::keyPressList;
std::list<sf::Keyboard::Key> IOManager::keyDownList;
std::list<sf::Keyboard::Key> IOManager::keyUpList;
std::deque<std::pair<sf::Keyboard::Key, IOManager::KEY_STATE>> IOManager::combo;
float IOManager::comboTimer = 0.f;
float IOManager::comboTimeLimit = 1.f;
bool IOManager::doComboRecord = false;
bool IOManager::isMouseInWindow = false;
float IOManager::wheelDelta = 0;
std::wstring IOManager::inputText;
bool IOManager::doInputText = false;
size_t IOManager::textSize = 12;

void IOManager::KeyEventUpdate(const sf::Event& event)
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
	case sf::Event::MouseButtonPressed:
		if (!IsKeyPress(event.mouseButton.button))
		{
			keyPressList.push_back(MouseKeyConversion(event.mouseButton.button));
			keyDownList.push_back(MouseKeyConversion(event.mouseButton.button));
			if (doComboRecord)
				combo.push_back({ MouseKeyConversion(event.mouseButton.button), KEY_STATE::DOWN });
		}
		break;
	case sf::Event::MouseButtonReleased:
		keyPressList.remove(MouseKeyConversion(event.mouseButton.button));
		keyUpList.push_back(MouseKeyConversion(event.mouseButton.button));
		if (doComboRecord)
			combo.push_back({ MouseKeyConversion(event.mouseButton.button), KEY_STATE::UP });
		break;
	case sf::Event::MouseLeft:
		isMouseInWindow = false;
		break;
	case sf::Event::MouseEntered:
		isMouseInWindow = true;
		break;
	case sf::Event::MouseWheelScrolled:
		wheelDelta = event.mouseWheelScroll.delta;
		break;
	case sf::Event::TextEntered:
		if (doInputText)
		{
			if (event.text.unicode < 0x0020)
			{
				switch (event.text.unicode)
				{
				case 0x0008:
					if (!inputText.empty())
						inputText.pop_back();
					break;
				case 0x000D:
					doInputText = false;
					break;
				}
				break;
			}
			else if (inputText.size() < textSize && event.text.unicode <= 0x007E)
			{
				switch (event.text.unicode)
				{
				case 0x005C: // "\"
				case 0x002F: // "/"
				case 0x003A: // ":"
				case 0x002A: // "*"
				case 0x003F: // "?"
				case 0x0022: // (")
				case 0x003C: // "<"
				case 0x003E: // ">"
				case 0x007C: // "|"
					break;
				default:
					inputText += event.text.unicode;
					break;
				}
			}
		}
		break;
	}
}

void IOManager::KeyUpdate(float timedetla, float timeScale)
{
	keyDownList.clear();
	keyUpList.clear();
	wheelDelta = 0.f;
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

sf::Keyboard::Key IOManager::MouseKeyConversion(const sf::Mouse::Button mouse)
{
	return sf::Keyboard::Key(mouse + sf::Keyboard::KeyCount);
}

sf::Mouse::Button IOManager::MouseKeyConversion(const sf::Keyboard::Key key)
{
	return sf::Mouse::Button(key - sf::Keyboard::KeyCount);
}

bool IOManager::IsKeyPress(const sf::Mouse::Button mouse)
{
	return IsKeyPress(MouseKeyConversion(mouse));
}

bool IOManager::IsKeyDown(const sf::Mouse::Button mouse)
{
	return IsKeyDown(MouseKeyConversion(mouse));
}

bool IOManager::IsKeyUp(const sf::Mouse::Button mouse)
{
	return IsKeyUp(MouseKeyConversion(mouse));
}

float IOManager::GetWheelDelta()
{
	return wheelDelta;
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

void IOManager::SetDoInputText(bool value, size_t textSize)
{
	doInputText = value;
	IOManager::textSize = textSize;
}









//////////////////////////////////////////////////////////////////////////////////////////////
//    
//     Sound
// 
//////////////////////////////////////////////////////////////////////////////////////////////
std::pair<std::pair<std::string, sf::Sound>, std::pair<std::string, sf::Sound>> IOManager::bgm[2];

std::list<std::shared_ptr<sf::Sound>> IOManager::usingSfx;
std::list<std::shared_ptr<sf::Sound>> IOManager::waitingSfx;

void IOManager::SoundInit(int sfxCount)
{
	for (int i = 0; i < sfxCount; i++)
	{
		waitingSfx.push_back(std::make_shared<sf::Sound>());
	}
}

void IOManager::SoundUpdate(float timeDelta, float timeScale)
{
	auto it = usingSfx.begin();
	while (it != usingSfx.end())
	{
		if ((*it)->getStatus() == sf::Sound::Stopped)
		{
			waitingSfx.push_back((*it));
			it = usingSfx.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void IOManager::SoundRelease()
{
	bgm[0].first.second.stop();
	bgm[0].second.second.stop();
	bgm[1].first.second.stop();
	bgm[1].second.second.stop();

	for (auto& ptr : usingSfx)
	{
		ptr->stop();
	}
	usingSfx.clear();
	waitingSfx.clear();

}

void IOManager::PlayBGMCh1(const std::string& path, bool loop)
{
	if (bgm->first.first != path)
	{
		bgm->first.second.stop();
		bgm->first.second.setBuffer(SFGM_SOUNDBUFFER.Get(path));
		bgm->first.first = path;
		bgm->first.second.play();
	}
	bgm->first.second.setLoop(loop);
}

void IOManager::PlayBGMCh2(const std::string& path, bool loop)
{
	if (bgm->second.first != path)
	{
		bgm->second.second.stop();
		bgm->second.second.setBuffer(SFGM_SOUNDBUFFER.Get(path));
		bgm->second.first = path;
		bgm->second.second.play();
	}
	bgm->second.second.setLoop(loop);
}


void IOManager::BGMSyncPlay(const std::string& path1, const std::string& path2, bool loop)
{
	if (bgm->first.first != path1)
	{
		bgm[0].first.second.stop();
		bgm[0].first.second.setBuffer(SFGM_SOUNDBUFFER.Get(path1));
		bgm[0].first.first = path1;
	}
	if (bgm->second.first != path2)
	{
		bgm[0].second.second.stop();
		bgm[0].second.second.setBuffer(SFGM_SOUNDBUFFER.Get(path2));
		bgm[0].second.first = path2;
	}
	bgm[0].first.second.setLoop(loop);
	bgm[0].second.second.setLoop(loop);

	bgm[0].first.second.play();
	bgm[0].second.second.play();
}

void IOManager::BGMSyncPlay(const std::string& path1, const std::string& path2, unsigned int channel, bool loop)
{
	BGMSyncSwitch(channel);
	BGMSyncPlay(path1, path2, loop);
}

void IOManager::BGMSyncSwitch(unsigned int channel)
{
	if (channel <= 1)
	{
		bgm[0].first.second.setVolume(100.f);
		bgm[0].second.second.setVolume(0.f);
	}
	else
	{
		bgm[0].second.second.setVolume(100.f);
		bgm[0].first.second.setVolume(0.f);
	}
	SetBGMVolume();
}

std::shared_ptr<sf::Sound> IOManager::PlaySfx(const std::string& path, bool listener, bool play)
{
	std::shared_ptr<sf::Sound> sfx;
	if (waitingSfx.empty())
	{
		sfx = usingSfx.front();
		usingSfx.pop_front();
		sfx->stop();
	}
	else
	{
		sfx = waitingSfx.front();
		waitingSfx.pop_front();
	}

	sfx->setBuffer(SFGM_SOUNDBUFFER.Get(path));
	sfx->setPosition(0.f, 0.f, 0.f);
	sfx->setMinDistance(1);
	sfx->setAttenuation(1);
	sfx->setRelativeToListener(listener);
	usingSfx.push_back(sfx);
	if(play)
		sfx->play();
	return sfx;
}

float IOManager::PlaySfx(const std::string& path, sf::Vector3f pos, float minD, float atte)
{
	std::shared_ptr<sf::Sound> sfx = PlaySfx(path, false, false);
	sfx->setPosition(pos);
	sfx->setMinDistance(minD);
	sfx->setAttenuation(atte);
	sfx->play();
	return sfx->getBuffer()->getDuration().asSeconds();
}

void IOManager::SetBGMCh1Volume(float volume)
{
	bgm[0].first.second.setVolume(volume);
	bgm[1].first.second.setVolume(volume);
	SetBGMVolume();
}

void IOManager::SetBGMCh2Volume(float volume)
{
	bgm[0].second.second.setVolume(volume);
	bgm[1].second.second.setVolume(volume);
	SetBGMVolume();
}

void IOManager::SetBGMVolume()
{
	bgm[0].first.second.setVolume(bgm[0].first.second.getVolume() * 0.7f);
	bgm[0].second.second.setVolume(bgm[0].second.second.getVolume() * 0.7f);
	bgm[1].first.second.setVolume(bgm[1].first.second.getVolume() * 0.7f);
	bgm[1].second.second.setVolume(bgm[1].second.second.getVolume() * 0.7f);
}

void IOManager::StopBGM(unsigned int channel)
{
	if (channel <= 1)
	{
		bgm[0].first.second.stop();
		bgm[1].first.second.stop();
	}
	else
	{
		bgm[0].second.second.stop();
		bgm[1].second.second.stop();
	}
}

void IOManager::StopBGM()
{
	bgm[0].first.second.stop();
	bgm[0].second.second.stop();
	bgm[1].first.second.stop();
	bgm[1].second.second.stop();
}

void IOManager::SetVolume(float volume)
{
	sf::Listener::setGlobalVolume(volume);
}