#pragma once

#define CITY_RCI (RCIManager::Instance())

struct RCI
{
	int residence = 0; //주거
	int commerce = 0; //상업
	int industry = 0; //산업

};

class RCIManager final
{
private:
	int totalRegidence;
	int totalCommerce;
	int totalIndustry;

	RCIManager() = default;
public:
	~RCIManager() = default;

	static RCIManager& Instance()
	{
		static RCIManager instance;
		return instance;
	}

	inline int UpdateRegidence(int value) { return totalRegidence += value; }
	inline int UpdateCommerce(int value) { return totalCommerce += value; }
	inline int UpdateIndustry(int value) { return totalIndustry += value; }
	void UpdateRCI(int r, int c, int i);

};

