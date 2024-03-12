#include "pch.h"
#include "RCIManager.h"
#include <SceneGameUI.h>

void RCIManager::Reset()
{
	totalRegidence = 0;
	totalCommerce = 0;
	totalIndustry = 0;

	usingRegidence = 0;
	usingCommerce = 0;
	usingIndustry = 0;

	needCommerce = 0;
}

void RCIManager::UpdateRCI() const
{
	std::string str = "R: " + std::to_string(totalRegidence) + " " + std::to_string(usingRegidence) + " " + std::to_string(totalRegidence - usingRegidence)
		+ "\nC: "+ std::to_string(totalCommerce) + " " + std::to_string(usingCommerce) + " " + std::to_string(totalCommerce - usingCommerce)
		+ "\nI: "+ std::to_string(totalIndustry) + " " + std::to_string(usingIndustry) + " " + std::to_string(totalIndustry - usingIndustry);


	std::dynamic_pointer_cast<SceneGameUI,Scene>(SceneManager::Get("SceneGameUI"))
		->SetTempText(str);
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

void RCIManager::INeedCommerce(int value)
{
	needCommerce += value;
}

void RCIManager::UseIndustry(int value)
{
	usingIndustry += value;
	UpdateRCI();
}
