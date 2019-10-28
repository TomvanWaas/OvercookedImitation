#include "stdafx.h"
#include "StaticInfo.h"



void StaticInfo::AddInfo(const HolderInfo& info)
{
	auto it = m_HolderData.find(info.type);
	if (it == m_HolderData.end())
	{
		m_HolderData[info.type] = info;
	}
}
void StaticInfo::AddInfo(const FoodInfo& info)
{
	auto it = m_FoodData.find(info.type);
	if (it == m_FoodData.end())
	{
		m_FoodData[info.type] = info;
	}
}

const HolderInfo& StaticInfo::GetInfo(eHolderType type) const
{
	auto it = m_HolderData.find(type);
#ifdef _DEBUG
	if (it == m_HolderData.end()) Logger::LogError(L"StaticInfo::GetInfo >> Invalid HolderType: " + std::to_wstring(int(type)));
#endif
	return it->second;
}
const FoodInfo& StaticInfo::GetInfo(eFoodType type) const
{
	auto it = m_FoodData.find(type);
#ifdef _DEBUG
	if (it == m_FoodData.end()) Logger::LogError(L"StaticInfo::GetInfo >> Invalid FoodType: " + std::to_wstring(int(type)));
#endif
	return it->second;
}
