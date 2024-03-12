#pragma once
#include "ObjectButton.h"

class ButtonNameTag : public ObjectButton
{
protected:
	sf::Text text;

	sf::RectangleShape background;
	sf::CircleShape backgroundCircle;

	bool isTextField = true;
	bool isInputMode = false;

public:
	explicit ButtonNameTag(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::function<void()>& func = nullptr);
	ButtonNameTag(const ButtonNameTag&) = delete;
	ButtonNameTag(ButtonNameTag&&) = delete;
	ButtonNameTag& operator=(const ButtonNameTag&) = delete;
	ButtonNameTag& operator=(ButtonNameTag&&) = delete;

	void Init() override;
	void Update(float timeDelta, float timeScale) override;
	void Draw(sf::RenderWindow& window) override;
	void SetPosition(const sf::Vector2f& position) override;
	void SetString(const std::string& str);
	void SetString(const std::wstring& wstr);
	void SetOrigin(const sf::Vector2f& origin) override;
	void SetOrigin(ORIGIN origin) override;
	void SetWidth(float width);

	inline void SetIsTextField(bool value) { isTextField = value; }
	void SetInputMode(bool value, size_t textSize = 6);

	inline const std::string& GetString() { return text.getString(); }
	inline bool IsInputMode() const { return isInputMode; }

	void OnDown() override;

	static std::shared_ptr<ButtonNameTag> Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::string& text = "", const std::function<void()>& func = nullptr);
	static std::shared_ptr<ButtonNameTag> Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::wstring& text, const std::function<void()>& func = nullptr);

};

