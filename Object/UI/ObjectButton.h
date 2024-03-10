#pragma once
#include "GameObject.h"
class ObjectButton : public GameObject
{
protected:
	sf::RenderStates renderStates;
	sf::Vector2u textureSize;
	sf::Sprite icon;

	sf::Text text;
	sf::VertexArray nineSlicing;
	sf::FloatRect bound;

	sf::Vector2f origin;
	sf::Vector2f scale = {0.5f,0.5f};
	float rotate = 0.f;

	sf::Color color = {150,120,100,220};
	
	int state = 0; //0:NONE, 1:HOVER, 2:DOWN

	std::function<void()> func;
	sf::Sound hover;
	sf::Sound click;

public:
	explicit ObjectButton(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::string& str ="", const std::function<void()>& func = nullptr);
	explicit ObjectButton(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::wstring& str =L"", const std::function<void()>& func = nullptr);
	ObjectButton(const ObjectButton&) = delete;
	ObjectButton(ObjectButton&&) = delete;
	ObjectButton& operator=(const ObjectButton&) = delete;
	ObjectButton& operator=(ObjectButton&&) = delete;

	void Init() override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;

	void SetOrigin(ORIGIN origin);
	void SetOrigin(sf::Vector2f origin);
	void SetPosition(const sf::Vector2f& position) override;
	void SetState(int state);

	void SetSize(sf::Vector2f size);

	void SetString(const std::string& str);
	void SetString(const std::wstring& wstr);

	void SetIcon(const std::string& path);
	void SetClickFunc();

	void ContentsUpdate();
	//void SetSize() override;
};

