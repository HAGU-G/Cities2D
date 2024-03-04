#pragma once
/// <summary>
/// RenderWindow resize by ratio.
/// sf::RenderWindow와 같은 역할을 한다. 비율에 따라 렌더링 창 크기가 조정된다. 비율은 create됐을 때 정해진다.
/// </summary>
class RenderWindowRBR : public sf::RenderWindow
{
protected:

	bool isResizing = false;

	sf::Vector2u sizeRatio;
	sf::Vector2u preSize;

	void onCreate() override;
	void onResize() override;

};

