#include "stdafx.h"
#include "BoxComponentBase.h"
#include "Observed.h"
#include "../../Helpers/Helpers.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include "Destroyer.h"
#include "../Holder/HolderInterfaceComponent.h"
#include "../Item/ItemInterfaceComponent.h"
#include "../../Helpers/Events.h"
#include "../../Materials/ColorMaterial.h"
#include "SoundManager.h"
#include "../../Helpers/SoundID.h"

BoxComponentBase::BoxComponentBase(eBoxType type, 
	const DirectX::XMFLOAT3& objectOffset,
	const std::vector<eHolderType>& allowedHolders,
	const ModelInfo& initInfo)
	: m_Type(type)
	, m_HolderPointer()
	, m_pMaterial(nullptr)
	, m_AllowedHolders(allowedHolders)
	, m_ObjectOffset(objectOffset)
	, m_IsHighlighted(false)
	, m_ModelInfo(initInfo)
{
	//If Allowed Does not contain 'Empty'=> Add Empty
	eHolderType baseType = eHolderType::Empty;
	auto it = std::find(m_AllowedHolders.begin(), m_AllowedHolders.end(), baseType);
	if (it == m_AllowedHolders.end())
	{
		m_AllowedHolders.push_back(baseType);
	}
}

void BoxComponentBase::Highlight(const DirectX::XMFLOAT4& color)
{
	m_IsHighlighted = true;
	if (m_pMaterial) m_pMaterial->SetColor(color);
	EnableHighlight(color);
	
	auto* pHolderObject = m_HolderPointer.Get();
	if (pHolderObject) pHolderObject->Highlight(color);

	auto* pItemObject = m_ItemPointer.Get();
	if (pItemObject) pItemObject->Highlight(color);
}

bool BoxComponentBase::Interact(float)
{
	return false;
}
bool BoxComponentBase::AddFood(HolderPointer& other)
{
	if (m_ItemPointer.IsValid()) return false;
	//Validate Holders
	//****************
	if (m_HolderPointer.IsValid() && other.IsValid())
	{
		const bool emptySelf = m_HolderPointer.IsEmpty();
		const bool emptyOther = other.IsEmpty();
		if (emptySelf && emptyOther) return false;

		//If Empty
		//********
		if (emptySelf)
		{
			//=> Check HolderType
			//*******************
			auto type = other.Get()->GetType();
			auto it = std::find(m_AllowedHolders.begin(), m_AllowedHolders.end(), type);
			if (it == m_AllowedHolders.end()) return false;

			//=> Swap
			//*******
			m_HolderPointer.Swap(other);
			if (m_CaughtFire) m_HolderPointer.Get()->Burn();
			AddSound();
			return true;
		}
		//Else => Add
		//***********
		if ( m_HolderPointer.Get()->AddFood(*other.Get()))
		{
			if (m_CaughtFire) m_HolderPointer.Get()->Burn();
			AddSound();
			return true;
		}
	}
	return false;
}
bool BoxComponentBase::TakeFood(HolderPointer& other)
{
	//Validate Holders
	//****************
	if (m_HolderPointer.IsValid() && other.IsValid())
	{
		const bool emptySelf = m_HolderPointer.IsEmpty();
		const bool emptyOther = other.IsEmpty();
		if (emptySelf && emptyOther) return false;

		//If Empty
		//********
		if (emptyOther)
		{
			//=> Check HolderType
			//*******************
			auto type = other.Get()->GetType();
			auto it = std::find(m_AllowedHolders.begin(), m_AllowedHolders.end(), type);
			if (it == m_AllowedHolders.end()) return false;

			//Swap
			//****
			other.Swap(m_HolderPointer);
			TakeSound();
			return true;
		}
		//Else => Add
		//***********
		if (other.Get()->AddFood(*m_HolderPointer.Get()))
		{
			TakeSound();
		}
	}
	return false;
}

bool BoxComponentBase::AddItem(ItemPointer& other)
{
	if (m_HolderPointer.IsEmpty() && !m_ItemPointer.IsValid())
	{
		m_ItemPointer.Set(other.Remove());
		AddSound();
		return true;
	}
	return false;
}
bool BoxComponentBase::TakeItem(ItemPointer& other)
{
	if (m_ItemPointer.IsValid() && !other.IsValid())
	{
		other.Set(m_ItemPointer.Remove());
		TakeSound();
		return true;
	}
	return false;
}

bool BoxComponentBase::InitializeItem(ItemInterfaceComponent* pItem)
{
	if ( (m_HolderPointer.IsEmpty() || !m_HolderPointer.IsValid()) && !m_ItemPointer.IsValid() && pItem)
	{
		m_ItemPointer.Initialize(GetGameObject(), m_ObjectOffset);
		auto* pOld = m_ItemPointer.Set(pItem);
		if (pOld) Destroyer::GetInstance().Destroy(pOld->GetGameObject());
		return true;
	}
	return false;
}
bool BoxComponentBase::InitializeHolder(HolderInterfaceComponent* pHolder)
{
	if (!m_HolderPointer.IsValid() && pHolder)
	{
		m_HolderPointer.Initialize(GetGameObject(), m_ObjectOffset);
		HolderInterfaceComponent* pPrevious = m_HolderPointer.Set(pHolder);
		if (pPrevious) Destroyer::GetInstance().Destroy(pPrevious->GetGameObject());
		if (m_CaughtFire) pHolder->Burn();
		return true;
	}
	return false;
}

void BoxComponentBase::OnNotify(ObservedID observed)
{
	switch (observed.id)
	{
	case Events::EVENT_BOX_FIRE_STARTED:
	{
		m_CaughtFire = true;
		if (m_HolderPointer.IsValid())
		{
			Logger::LogInfo(L"BoxBaseComponent::OnNotify >> Fire Started");
			auto* pHolder = m_HolderPointer.Get();
			pHolder->Burn();
		}
	}
	break;
	case Events::EVENT_BOX_FIRE_ENDED:
	{
		m_CaughtFire = false;
		Logger::LogInfo(L"BoxBaseComponent::OnNotify >> Fire Ended");
	}
	break;
	}
}
void BoxComponentBase::Initialize(const GameContext& gameContext)
{
	InitializeHolder(eHolderType::Empty);
	InitializeModel(gameContext);
	m_ItemPointer.Initialize(GetGameObject(), m_ObjectOffset);
}
void BoxComponentBase::Update(const GameContext&)
{
}
void BoxComponentBase::Draw(const GameContext&)
{
}
void BoxComponentBase::PostDraw(const GameContext&)
{
	if (m_IsHighlighted)
	{
		m_IsHighlighted = false;
		if (m_pMaterial) m_pMaterial->SetColor({ 1,1,1,1 });
		DisableHighlight();
	}
}

void BoxComponentBase::EnableHighlight(const DirectX::XMFLOAT4&)
{
	//Empty
}
void BoxComponentBase::DisableHighlight()
{
	//Empty
}

void BoxComponentBase::InitializeHolder(eHolderType type)
{
	m_HolderPointer.Initialize(GetGameObject(), m_ObjectOffset);
	if (!m_HolderPointer.IsValid() && Helpers::AddHolder(m_HolderPointer, type))
	{
		Logger::LogInfo(L"BoxComponent::Initialize >> Invalid Holder, Added Holder");
	}
}
void BoxComponentBase::InitializeModel(const GameContext& gameContext)
{
	//Add Components
	//**************
	auto* pObject = GetGameObject();

	//ModelComponent
	//**************
	auto modelpair = m_ModelInfo.MakeDiffuseModel(gameContext);
	ModelComponent* pModel = modelpair.first;
	m_pMaterial = static_cast<ColorMaterial*>(modelpair.second.first);
	pObject->AddComponent(pModel);
}




void BoxComponentBase::AddSound()
{
	SoundManager::GetInstance()->PlaySoundByID(int(eSoundID::DropSingle), int(eChannelID::Effect01), 0.025f, eSoundMerge::Overlap);
}
void BoxComponentBase::TakeSound()
{
	SoundManager::GetInstance()->PlaySoundByID(int(eSoundID::TakeSingle), int(eChannelID::Effect01), 0.025f, eSoundMerge::Overlap);
}
