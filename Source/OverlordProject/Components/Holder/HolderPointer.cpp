#include "stdafx.h"
#include "HolderPointer.h"
#include "GameObject.h"
#include "GameScene.h"
#include "TransformComponent.h"
#include "HolderComponent.h"
#include "HolderInterfaceComponent.h"

HolderInterfaceComponent* HolderPointer::Get() const
{
	return m_pHolder;
}
GameObject* HolderPointer::GetParent() const
{
	return m_pParent;
}

HolderInterfaceComponent* HolderPointer::Set(HolderInterfaceComponent* pHolder)
{
	HolderInterfaceComponent* pPrevious = m_pHolder;
	HelperRemove();
	HelperAdd(pHolder);
	return pPrevious;
}
HolderInterfaceComponent* HolderPointer::Remove()
{
	HolderInterfaceComponent* pPrevious = m_pHolder;
	HelperRemove();
	return pPrevious;
}
void HolderPointer::Swap(HolderPointer& other)
{
	HolderInterfaceComponent* pPreviousSelf = m_pHolder;
	HolderInterfaceComponent* pPreviousOther = other.m_pHolder;
	HelperRemove();
	other.HelperRemove();
	HelperAdd(pPreviousOther);
	other.HelperAdd(pPreviousSelf);
}

void HolderPointer::Initialize(GameObject* pParent, const DirectX::XMFLOAT3& holderOffset)
{
	m_pParent = pParent;
	m_Offset = holderOffset;
}

bool HolderPointer::Initialized() const
{
	return bool(m_pParent);
}
bool HolderPointer::IsValid() const
{
	return bool(m_pHolder);
}

bool HolderPointer::IsEmpty() const
{
	//Valid Empty Type Holder with no food
	return IsValid() && m_pHolder->GetHolderComponent() && m_pHolder->GetHolderComponent()->GetIndex() == 0
		&& m_pHolder->GetType() == eHolderType::Empty;
}


void HolderPointer::HelperRemove()
{
	if (m_pHolder && m_pParent)
	{
		m_pParent->RemoveChild(m_pHolder->GetGameObject());
		m_pHolder = nullptr;
	}
}
void HolderPointer::HelperAdd(HolderInterfaceComponent* pHolder)
{
	if (pHolder && m_pParent)
	{
		GameObject* pObject = pHolder->GetGameObject();
		DirectX::XMFLOAT3 wscale = pObject->GetTransform()->GetWorldScale();

		//Remove from Scene &| Parent
		GameObject* pParent = pObject->GetParent();
		if (pParent)
		{
			pParent->RemoveChild(pObject);
		}
		GameScene* pScene = pObject->GetScene();
		if (pScene)
		{
			pScene->RemoveChild(pObject, false);
		}

		//Add
		m_pParent->AddChild(pObject);

		//Set Transform
		pObject->GetTransform()->Translate(m_Offset);
		pObject->GetTransform()->Rotate(0, randF(0.0f, 360.0f), 0, true);
		pObject->GetTransform()->WorldScale(wscale);

		m_pHolder = pHolder;
	}
}
