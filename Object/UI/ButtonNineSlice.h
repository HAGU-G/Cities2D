#pragma once
#include "ObjectButton.h"
class ButtonNineSlice : public ObjectButton
{
protected:
	sf::RenderStates renderStates;
	sf::Vector2u textureSize;
	sf::VertexArray nineSlicing;
	sf::Text text;

public:
	explicit ButtonNineSlice(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::string& str = "", const std::function<void()>& func = nullptr);
	explicit ButtonNineSlice(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::wstring& str = L"", const std::function<void()>& func = nullptr);
	ButtonNineSlice(const ButtonNineSlice&) = delete;
	ButtonNineSlice(ButtonNineSlice&&) = delete;
	ButtonNineSlice& operator=(const ButtonNineSlice&) = delete;
	ButtonNineSlice& operator=(ButtonNineSlice&&) = delete;

	void Init() override;
	void Draw(sf::RenderWindow& window) override;

	void SetOrigin(ORIGIN origin) override;
	void SetPosition(const sf::Vector2f& position) override;
	void SetState(int state) override;

	void SetSize(sf::Vector2f size);

	void ContentsUpdate();

	static std::shared_ptr<ButtonNineSlice> Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::string& str = "", const std::function<void()>& func = nullptr);
	static std::shared_ptr<ButtonNineSlice> Create(std::weak_ptr<Scene> scene, sf::Vector2f position, const std::string& iconPath, const std::wstring& str, const std::function<void()>& func = nullptr);

	void SetString(const std::string& str);

	void SetString(const std::wstring& wstr);

};

