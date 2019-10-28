#include "stdafx.h"
#include "BoxComponentHolderContainer.h"
#include "../../Singletons/Templates.h"
#include "TransformComponent.h"
#include "../../Prototype/PrototypeObject.h"
#include "Destroyer.h"
#include "../Holder/HolderInterfaceComponent.h"

BoxComponentHolderContainer::BoxComponentHolderContainer(
	eBoxType type, 
	const DirectX::XMFLOAT3& holderOffset,
	const std::vector<eHolderType>& allowedHolders, 
	const ModelInfo& initInfo, 
	eHolderType templateType,
	int templateCount)
	: BoxComponentBase(type, holderOffset, allowedHolders, initInfo)
	, m_TemplateType(templateType)
	, m_TemplateCount(templateCount)
{
}

void BoxComponentHolderContainer::AddTemplateCount(int count)
{
	if (m_TemplateCount >= 0)
	{
		m_TemplateCount += count;
	}
}

bool BoxComponentHolderContainer::AddFood(HolderPointer&)
{
	return false;
}
bool BoxComponentHolderContainer::TakeFood(HolderPointer& other)
{
	//Requirement
	//***********
	if (!IsOnFire() && other.IsEmpty() && m_pCurrentClone)
	{
		//Remove As Child
		//***************
		auto* pObject = GetGameObject();
		auto* pClone = m_pCurrentClone->GetGameObject();
		pObject->RemoveChild(pClone);
	
		//Add To Other
		//************				
		auto* pEmpty = other.Set(m_pCurrentClone);
		if (pEmpty) Destroyer::GetInstance().Destroy(pEmpty->GetGameObject());

		//Make New
		//********
		m_pCurrentClone = nullptr;
		AddTemplateClone();
		TakeSound();
		return  true;
	}
	return false;
}

void BoxComponentHolderContainer::Initialize(const GameContext& gameContext)
{
	//Add Template Holder Object
	//**************************
	AddTemplateClone();

	//Base Initialize
	//***************
	BoxComponentBase::Initialize(gameContext);
}

void BoxComponentHolderContainer::Update(const GameContext& gameContext)
{
	if (m_pCurrentClone)
	{
		const float size = 6.0f;
		const float speed = 10.0f;
		const float elapsed = gameContext.pGameTime->GetElapsed();

		//Transform
		//*********
		const float offset = speed * elapsed;
		auto* pTransform = m_pCurrentClone->GetTransform();
		const auto& currentPosition = pTransform->GetPosition();
		if (currentPosition.y + FLT_EPSILON < size)
		{
			DirectX::XMFLOAT3 newPosition = currentPosition;
			newPosition.y += offset;
			if (newPosition.y > size) newPosition.y = size;
			pTransform->Translate(newPosition);
		}
	}
}

void BoxComponentHolderContainer::AddTemplateClone()
{
	if (m_pCurrentClone == nullptr && m_TemplateCount != 0)
	{
		--m_TemplateCount;

		auto* pObject = Templates::GetInstance().GetClone(m_TemplateType);
		m_pCurrentClone = (pObject) ? (pObject->GetDynamicComponent<HolderInterfaceComponent>()) : nullptr;
		if (m_pCurrentClone)
		{
			GetGameObject()->AddChild(pObject);
			return;
		}
		if (pObject) Destroyer::GetInstance().Destroy(pObject);
		m_pCurrentClone = nullptr;
	}
}

