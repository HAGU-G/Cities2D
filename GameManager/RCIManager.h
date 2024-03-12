#pragma once

#define SFGM_RCI (RCIManager::Instance())
class ObjectUnit;

struct RCI
{
	int residence = 0; //주거
	int commerce = 0; //상업
	int industry = 0; //산업

	int cost = 0;
	int tex = 0;

	std::unordered_map<std::string, std::weak_ptr<ObjectUnit>> residenceSlot;
	std::unordered_map<std::string, std::weak_ptr<ObjectUnit>> commerceSlot;
	std::unordered_map<std::string, std::weak_ptr<ObjectUnit>> industrySlot;

};

class RCIManager final
{
private:
	int totalRegidence = 0;
	int totalCommerce = 0;
	int totalIndustry = 0;

	int usingRegidence = 0;
	int usingCommerce = 0;
	int usingIndustry = 0;

	int needCommerce = 0;

	RCIManager() = default;
public:
	~RCIManager() = default;

	void Reset();

	void UpdateRCI() const;
	void UpdateRCI(int r, int c, int i);

	void UpdateRegidence(int value);
	void UpdateCommerce(int value);
	void UpdateIndustry(int value);

	void UseRegidence(int value);
	void UseCommerce(int value);
	void INeedCommerce(int value);
	void UseIndustry(int value);

	int GetUsingRegidence() const { return usingRegidence; }
	int GetUsingCommerce() const { return usingCommerce; }
	int GetUsingIndustry() const { return usingIndustry; }

	int LeftRegidence() const { return totalRegidence - usingRegidence; }
	int LeftCommerce() const { return totalCommerce - usingCommerce; }
	int LeftIndustry() const { return totalIndustry - usingIndustry; }

	int NeedRegidence() const { return std::max(1, std::max(LeftCommerce() - (needCommerce- usingCommerce), LeftIndustry()) - LeftRegidence()); }
	int NeedCommerce() const { return std::max(0, needCommerce - usingCommerce - LeftCommerce()); }
	int NeedIndustry() const { return std::max(0, usingRegidence - usingIndustry); }

	static RCIManager& Instance()
	{
		static RCIManager instance;
		return instance;
	}
};

