#include "pch.h"
#include "RenderWindowRBR.h"

void RenderWindowRBR::onCreate()
{
	RenderWindow::onCreate();
	preSize = sizeRatio = getSize();
}

void RenderWindowRBR::onResize()
{
	RenderWindow::onResize();
	if (isResizing)
		return;

	isResizing = true;

	sf::Vector2u currentSize = getSize();

	sf::Vector2u basedOnY((float)sizeRatio.x / sizeRatio.y * currentSize.y, currentSize.y);
	sf::Vector2u basedOnX(currentSize.x, (float)sizeRatio.y / sizeRatio.x * currentSize.x);

	sf::Vector2f changeRate(abs((float)currentSize.x / preSize.x - 1.f), abs((float)currentSize.y / preSize.y - 1.f));
	if (changeRate.x >= changeRate.y)
	{
		setSize(basedOnX);
	}
	else
	{
		setSize(basedOnY);
	}

	sf::Event e;
	this->waitEvent(e);
	preSize = getSize();

	isResizing = false;
}