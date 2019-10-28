#include "stdafx.h"
#include "HolderComponent.h"
#include "GameObject.h"
#include "../Food/FoodInterfaceComponent.h"
#include "../Food/FoodComponent.h"
#include "../../Helpers/Events.h"

HolderComponent::HolderComponent(const HolderInfo& info)
	: m_HolderInfo(info)
	, m_Index(0)
	, m_pFoodPointers(nullptr)
{
}
HolderComponent::~HolderComponent()
{
	if (m_pFoodPointers) delete[] m_pFoodPointers;
	m_pFoodPointers = nullptr;
}


PrototypeComponent* HolderComponent::Clone() const
{
	return new HolderComponent(m_HolderInfo);
}

void HolderComponent::Highlight(const DirectX::XMFLOAT4& color)
{
	for (int i = 0; i < m_Index; ++i)
	{
		FoodInterfaceComponent* pInterface = m_pFoodPointers[i].Get();
		pInterface->Highlight(color);
	}
}

bool HolderComponent::AddFood(HolderComponent& other)
{
	//These Are Static to Prevent the requirement of executing
	//the 'CanContain' function twice, first to check if it is possible,
	//and second if possible to get the conversion state
	static const int SIZE = 4;
	static eFoodState STATES[SIZE];

	//Checks
	//******

	//Check Count
	//***********
	if (m_Index + other.m_Index > m_HolderInfo.maxIndex)
	{
		return false;
	}
#ifdef _DEBUG
	if (m_Index + other.m_Index > SIZE)
	{
		Logger::LogError(L"HolderComponent::AddFood >> Function does not support recipes bigger than " + std::to_wstring(SIZE));
		return false;
	}
#endif

	//Check Can Contain
	//*****************
	for (int i = 0; i < other.m_Index && i < SIZE; ++i)
	{
		auto* pFoodInterface = other.m_pFoodPointers[i].Get();
		eFoodType type = pFoodInterface->GetType();
		eFoodState state = pFoodInterface->GetState();

		if (!CanContain(type, state, STATES[i])) return false;
	}

	//Add Food
	//********
	for (int i = 0, j = other.m_Index; i < j && i < SIZE; ++i)
	{
		auto* pFoodInterface = other.m_pFoodPointers[i].Remove();

		//Take
		//****
		m_pFoodPointers[m_Index].Set(pFoodInterface);
		++m_Index;
		--other.m_Index;
		
		//Convert
		//*******
		if (STATES[i] != eFoodState::Any)
		{
			pFoodInterface->Convert(STATES[i]);
		}
	}

	//Notify
	//******
	other.GetGameObject()->Notify(Events::EVENT_HOLDER_FOOD_REMOVED);
	GetGameObject()->Notify(Events::EVENT_HOLDER_FOOD_ADDED);

	//Burned
	//******
	if (ContainsBurned()) BurnFood();
	return true;
}
bool HolderComponent::AddFood(FoodInterfaceComponent* pFood)
{
	if (!pFood) return false;

	//Check Count & Checks
	//********************
	eFoodState resultState{};
	if (m_Index < m_HolderInfo.maxIndex
		&& CanContain(pFood->GetType(), pFood->GetState(), resultState))
	{
		//Convert to resultState
		//**********************
		if (resultState != eFoodState::Any)
		{
			pFood->Convert(resultState);
		}

		//Add to FoodPointers
		//*******************
		m_pFoodPointers[m_Index].Set(pFood);
		++m_Index;
		GetGameObject()->Notify(Events::EVENT_HOLDER_FOOD_ADDED);

		//Burned
		//******
		if (ContainsBurned()) BurnFood();
		return true;
	}
	return false;
}
FoodInterfaceComponent* HolderComponent::RemoveFood()
{
	if (m_Index > 0)
	{
		--m_Index;
		FoodInterfaceComponent* pFood = m_pFoodPointers[m_Index].Remove();
		if (pFood) GetGameObject()->Notify(Events::EVENT_HOLDER_FOOD_REMOVED);
		return pFood;
	}
	return nullptr;
}

void HolderComponent::BurnFood()
{
	if (m_pFoodPointers)
	{
		for (int i = 0; i < m_Index; ++i)
			m_pFoodPointers[i].Get()->Convert(eFoodState::Burned);
	}
}


eConvertReturn HolderComponent::Convert(float elapsed, eUpdate update, BoxComponentBase*)
{
	//Has Food
	//********
	if (m_Index > 0)
	{
		//More Food => Slower Update
		//**************************
		const float combinedElapsed = elapsed / m_Index;

		eConvertReturn result = eConvertReturn::Finished;
		FoodConversion conversion{};
		
		//For each Food
		//*************
		for (int i = 0; i < m_Index; ++i)
		{
			auto* pFood = m_pFoodPointers[i].Get();
			auto returnState = pFood->Convert(combinedElapsed, update, this, conversion);

			switch (returnState)
			{
				//Return Invalid if Failed to convert
				//***********************************
			case eConvertReturn::Failed:
				return eConvertReturn::Failed;
				break;
				//If at least 1 is converting, result is converting
				//*************************************************
			case eConvertReturn::Converting:
				result = eConvertReturn::Converting;
				break;
			}
		}
		return result;
	}
	return eConvertReturn::Failed;
}



int HolderComponent::GetIndex() const
{
	return m_Index;
}
int HolderComponent::GetMaxIndex() const
{
	return m_HolderInfo.maxIndex;
}
Recipe HolderComponent::GetRecipe() const
{
	Recipe recipe{};
	int size = std::min(std::min(m_Index, Recipe::SIZE), m_HolderInfo.maxIndex);
	for (int i = 0; i < size; ++i)
	{
		auto* pFoodObject = m_pFoodPointers[i].Get();
		recipe.foodTypes[i] = pFoodObject->GetType();
		recipe.foodStates[i] = pFoodObject->GetState();
	}
	for (size; size < Recipe::SIZE; ++size)
	{
		recipe.foodTypes[size] = eFoodType::_None;
		recipe.foodStates[size] = eFoodState::None;
	}
	recipe.holderType = m_HolderInfo.type;
	recipe.Sort();
	return recipe;
}

const FoodPointer& HolderComponent::GetFoodPointer(int idx) const
{
	assert(idx < m_Index && idx < m_HolderInfo.maxIndex);
	return m_pFoodPointers[idx];
}


void HolderComponent::Initialize(const GameContext&)
{
	GameObject* pObject = GetGameObject();
	
	//Initialize FoodPointers
	//***********************
	m_pFoodPointers = new FoodPointer[m_HolderInfo.maxIndex];
	for (int i = 0; i < m_HolderInfo.maxIndex; ++i)
	{
		m_pFoodPointers[i].Initialize(pObject, m_HolderInfo.foodOffsetWorld);
	}

	//...
}
void HolderComponent::Update(const GameContext&)
{
}
void HolderComponent::Draw(const GameContext&)
{
}

bool HolderComponent::CanContain(eFoodType type, eFoodState state, eFoodState& result) const
{
	//Find Type
	//*********
	auto itType = m_HolderInfo.containments.find(type);
	if (itType == m_HolderInfo.containments.end())
	{
		itType = m_HolderInfo.containments.find(eFoodType::_Any);
	}
	if (itType != m_HolderInfo.containments.end())
	{
		//Find Required State
		//*******************
		auto itState = itType->second.find(state);
		if (itState == itType->second.end())
		{
			itState = itType->second.find(eFoodState::Any);
		}
		if (itState != itType->second.end())
		{
			//Write & Return
			//**************
			result = itState->second;
			return true;
		}
	}
	return false;
}

bool HolderComponent::ContainsBurned() const
{
	for (int i = 0; i < m_Index; ++i)
	{
		FoodPointer& food = m_pFoodPointers[i];
		if (food.IsValid() && food.Get()->GetState() == eFoodState::Burned) return true;
	}
	return false;
}


