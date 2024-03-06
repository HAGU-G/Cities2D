#include "pch.h"
#include "RCIManager.h"
#include <SceneGameUI.h>

void RCIManager::UpdateRCI() const
{
	std::dynamic_pointer_cast<SceneGameUI,Scene>(SceneManager::Get("SceneGameUI"))->UpdateRCIGraph(totalRegidence - usingRegidence, totalCommerce - usingCommerce, totalIndustry - usingIndustry);
}

void RCIManager::UpdateRCI(int r, int c, int i)
{
	UpdateRegidence(r);
	UpdateCommerce(c);
	UpdateIndustry(i);
	UpdateRCI();
}

void RCIManager::UpdateRegidence(int r)
{
	totalRegidence += r;
	UpdateRCI();
}

void RCIManager::UpdateCommerce(int c)
{
	totalCommerce += c;
	UpdateRCI();
}

void RCIManager::UpdateIndustry(int i)
{
	totalIndustry += i;
	UpdateRCI();
}

void RCIManager::UseRegidence(int value)
{
	usingRegidence += value;
	UpdateRCI();
}

void RCIManager::UseCommerce(int value)
{
	usingCommerce += value;
	UpdateRCI();
}

void RCIManager::UseIndustry(int value)
{
	usingIndustry += value;
	UpdateRCI();
}
