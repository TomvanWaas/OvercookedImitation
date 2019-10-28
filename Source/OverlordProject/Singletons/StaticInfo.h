#pragma once
#include "../Templates/SingletonObject.h"
#include "../Components/Holder/HolderInfo.h"
#include "../Components/Food/FoodInfo.h"

class StaticInfo final : public SingletonObject<StaticInfo>
{
public:
	StaticInfo() = default;
	virtual ~StaticInfo() = default;

	void AddInfo(const HolderInfo& info);
	void AddInfo(const FoodInfo& info);
	const HolderInfo& GetInfo(eHolderType type) const;
	const FoodInfo& GetInfo(eFoodType type) const;
private:
	std::map<eHolderType, HolderInfo> m_HolderData;
	std::map<eFoodType, FoodInfo> m_FoodData;

};

