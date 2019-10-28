#include "stdafx.h"
#include "Templates.h"
#include "../Prototype/PrototypeObject.h"

Templates::~Templates()
{
	for (auto& p : m_pFoodTemplates)
	{
		delete p.second;
	}
	m_pFoodTemplates.clear();
	for (auto& p : m_pHolderTemplates)
	{
		delete p.second;
	}
	m_pHolderTemplates.clear();
}

bool Templates::Add(eFoodType key, PrototypeObject* pTemplate)
{
	auto it = m_pFoodTemplates.find(key);
	if (it == m_pFoodTemplates.end())
	{
		m_pFoodTemplates[key] = pTemplate;
#ifdef _DEBUG
		Logger::LogInfo(L"Templates::Add >> Added Food Template on ID: " + std::to_wstring(int(key)));
#endif
		return true;
	}
	return false;
}
bool Templates::Add(eHolderType key, PrototypeObject* pTemplate)
{
	auto it = m_pHolderTemplates.find(key);
	if (it == m_pHolderTemplates.end())
	{
		m_pHolderTemplates[key] = pTemplate;
#ifdef _DEBUG
		Logger::LogInfo(L"Templates::Add >> Added Holder Template on ID: " + std::to_wstring(int(key)));
#endif
		return true;
	}
	return false;
}



PrototypeObject* Templates::GetClone(eFoodType key) const
{
	auto it = m_pFoodTemplates.find(key);
	if (it != m_pFoodTemplates.end() && it->second)
	{
#ifdef _DEBUG
		Logger::LogInfo(L"Templates::GetClone >> Returned Food Template Clone on ID: " + std::to_wstring(int(key)));
#endif
		return it->second->Clone(false);
	}
	return nullptr;
}
PrototypeObject* Templates::GetClone(eHolderType key) const
{
	auto it = m_pHolderTemplates.find(key);
	if (it != m_pHolderTemplates.end() && it->second)
	{
#ifdef _DEBUG
		Logger::LogInfo(L"Templates::GetClone >> Returned Holder Template Clone on ID: " + std::to_wstring(int(key)));
#endif
		return it->second->Clone(false);
	}
	return nullptr;
}


