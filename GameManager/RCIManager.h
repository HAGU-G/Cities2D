#pragma once

#define GM_RCI (RCIManager::Instance())
class ObjectUnit;

struct RCI
{
	int residence = 0; //주거
	int commerce = 0; //상업
	int industry = 0; //산업

	int cost = 0;

	std::unordered_map<std::string, std::weak_ptr<ObjectUnit>> residenceSlot;
	std::unordered_map<std::string, std::weak_ptr<ObjectUnit>> commerceSlot;
	std::unordered_map<std::string, std::weak_ptr<ObjectUnit>> industrySlot;

};

class RCIManager final
{
private:
	int totalRegidence;
	int totalCommerce;
	int totalIndustry;

	int usingRegidence;
	int usingCommerce;
	int usingIndustry;


	RCIManager() = default;
public:
	~RCIManager() = default;

	void UpdateRCI() const;
	void UpdateRCI(int r, int c, int i);

	void UpdateRegidence(int value);
	void UpdateCommerce(int value);
	void UpdateIndustry(int value);

	void UseRegidence(int value);
	void UseCommerce(int value);
	void UseIndustry(int value);

	int LeftRegidence() const { return totalRegidence - usingRegidence; }
	int LeftCommerce() const { return totalCommerce - usingCommerce; }
	int LeftIndustry() const { return totalIndustry - usingIndustry; }

	int NeedRegidence() const { return std::max(1, std::max(LeftCommerce(), LeftIndustry()) - LeftRegidence()); }
	int NeedCommerce() const { return std::max(0, LeftRegidence() - LeftIndustry()); }
	int NeedIndustry() const { return std::max(0, LeftRegidence() - LeftCommerce()); }

	static RCIManager& Instance()
	{
		static RCIManager instance;
		return instance;
	}
};

