#include "pch.h"
#include "RCIManager.h"

void RCIManager::UpdateRCI(int r, int c, int i)
{
	totalRegidence += r;
	totalCommerce += c;
	totalIndustry += i;
}
