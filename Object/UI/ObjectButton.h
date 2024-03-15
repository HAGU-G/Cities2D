#pragma once
#include "GameObject.h"
class ObjectButton : public GameObject
{
protected:
	sf::Sprite icon;
	std::string iconName;
	sf::FloatRect bound;

	sf::Vector2f origin;
	sf::Vector2f scale = {1.f,1.f};
	float rotate = 0.f;

	sf::Color color = {150,120,100,220};
	
	int state = 0; //0:NONE, 1:HOVER, 2:DOWN

	std::function<void()> funcVoid_void;
	sf::Sound mouseOn;
	sf::Sound click;
	bool doToggle = false;
	bool isOnlyDown = false;
	bool isSelect = false;
	bool canReact = true;

public:
	explicit ObjectButton(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::function<void()>& func = nullptr);
	ObjectButton(const ObjectButton&) = delete;
	ObjectButton(ObjectButton&&) = delete;
	ObjectButton& operator=(const ObjectButton&) = delete;
	ObjectButton& operator=(ObjectButton&&) = delete;

	void Init() override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;

	virtual void SetOrigin(ORIGIN origin);
	virtual void SetOrigin(const sf::Vector2f& origin);
	virtual void SetPosition(const sf::Vector2f& position) override;
	virtual void SetState(int state);
	void SetIcon(const std::string& path);
	inline void SetFuntion(std::function<void()> func) { funcVoid_void = func; }
	void SetDoToggle(bool value) { doToggle = value; }
	inline void SetOnlyDown(bool value) { isOnlyDown = value; }
	inline void SetCanReact(bool value) { canReact = value; }

	void UnSelect();
	void Select();
	virtual void OnDown();



	static std::shared_ptr<ObjectButton> Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::function<void()>& func = nullptr, bool toggle = true);

	inline int GetState() const { return state; }
	inline bool IsSelete() const { return isSelect; }
	inline const sf::FloatRect& GetBound() const { return bound; }

};

