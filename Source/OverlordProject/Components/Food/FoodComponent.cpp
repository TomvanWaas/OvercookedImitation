#include "stdafx.h"
#include "FoodComponent.h"
#include "GameObject.h"
#include "../../Helpers/Events.h"

FoodComponent::FoodComponent(const FoodInfo& info, eFoodState state)
	: m_FoodInfo(info)
	, m_Conversion({ 0.0f, eUpdate::None, eFoodState::None })
	, m_State(state)
	, m_Accu(0.0f)

{
}

PrototypeComponent* FoodComponent::Clone() const
{
	return new FoodComponent(m_FoodInfo, m_State);
}


void FoodComponent::ReduceOvertime(float elapsed)
{
	//Reduce Conversion Accu if already Finished to prevent Overtime
	if (m_Conversion.update != eUpdate::None && m_Accu >= m_Conversion.time)
	{
		m_Accu -= elapsed;
		if (m_Accu < m_Conversion.time) m_Accu = m_Conversion.time;
	}
}
eConvertReturn FoodComponent::Convert(eFoodState state)
{
	if (m_State == eFoodState::Burned) return eConvertReturn::Failed;
	SetState(state);
	return eConvertReturn::Finished;
}
eConvertReturn FoodComponent::Convert(float elapsed, eUpdate update, HolderComponent*, FoodConversion& conversion)
{
	if (m_State == eFoodState::Burned) return eConvertReturn::Failed;

	//Find Conversion if different Update
	//***********************************
	if (m_Conversion.update != update)
	{
		if (!FindConversion(update, m_Conversion)) return eConvertReturn::Failed;
		m_Accu = 0.0f;
	}

	//Update Conversion
	//*****************
	m_Accu += elapsed;
	if (m_Accu >= m_Conversion.time )
	{
		if (m_State != m_Conversion.state)
		{
			SetState(m_Conversion.state);
		}
		conversion = m_Conversion;
		return eConvertReturn::Finished;
	}
	conversion = m_Conversion;
	return eConvertReturn::Converting;
}
bool FoodComponent::CanConvert(eUpdate update) const
{
	FoodConversion holder;
	return m_State != eFoodState::Burned && FindConversion(update, holder);
}

void FoodComponent::Initialize(const GameContext&)
{
	GetGameObject()->Notify(Events::EVENT_FOOD_STATE_CHANGED);
}
void FoodComponent::Update(const GameContext&)
{
}
void FoodComponent::Draw(const GameContext&)
{
}

bool FoodComponent::FindConversion(eUpdate update, FoodConversion& holder) const
{
	//Find Own State
	//**************
	auto it = m_FoodInfo.conversions.find(m_State);
	if (it == m_FoodInfo.conversions.end())
	{
		//If Failed => Find Any State (No Requirement)
		//********************************************
		it = m_FoodInfo.conversions.find(eFoodState::Any);
	}
	if (it != m_FoodInfo.conversions.end())
	{
		//Find Update
		//***********
		for (const auto& conversion : it->second)
		{
			if (conversion.update == update)
			{
				holder = conversion;
				return true;
			}
		}
	}
	return false;
}
void FoodComponent::SetState(eFoodState state)
{
	if (m_State != state && m_State != eFoodState::Burned)
	{
#ifdef _DEBUG
		Logger::LogInfo(L"FoodComponent::SetState >> State Changed from StateID: " + std::to_wstring(int(m_State)) + L" to StateID: " + std::to_wstring(int(state)));
#endif
		m_State = state;
		GetGameObject()->Notify(Events::EVENT_FOOD_STATE_CHANGED);
	}
}
