#include "stdafx.h"
#include "ItemPointer.h"
#include "GameObject.h"
#include "GameScene.h"
#include "TransformComponent.h"
#include "ItemInterfaceComponent.h"

ItemInterfaceComponent* ItemPointer::Get() const
{
	return m_pItem;
}
GameObject* ItemPointer::GetParent() const
{
	return m_pParent;
}

ItemInterfaceComponent* ItemPointer::Set(ItemInterfaceComponent* pItem)
{
	ItemInterfaceComponent* pPrevious = m_pItem;
	HelperRemove();
	HelperAdd(pItem);
	return pPrevious;
}
ItemInterfaceComponent* ItemPointer::Remove()
{
	ItemInterfaceComponent* pPrevious = m_pItem;
	HelperRemove();
	return pPrevious;
}
void ItemPointer::Swap(ItemPointer& other)
{
	ItemInterfaceComponent* pPreviousSelf = m_pItem;
	ItemInterfaceComponent* pPreviousOther = other.m_pItem;
	HelperRemove();
	other.HelperRemove();
	HelperAdd(pPreviousOther);
	other.HelperAdd(pPreviousSelf);
}

void ItemPointer::Initialize(GameObject* pParent, const DirectX::XMFLOAT3& itemOffset)
{
	m_pParent = pParent;
	m_Offset = itemOffset;
}

bool ItemPointer::Initialized() const
{
	return bool(m_pParent);
}
bool ItemPointer::IsValid() const
{
	return bool(m_pItem);
}


void ItemPointer::HelperRemove()
{
	if (m_pItem && m_pParent)
	{
		m_pParent->RemoveChild(m_pItem->GetGameObject());
		m_pItem = nullptr;
	}
}
void ItemPointer::HelperAdd(ItemInterfaceComponent* pItem)
{
	if (pItem && m_pParent)
	{
		GameObject* pObject = pItem->GetGameObject();
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
		pObject->GetTransform()->WorldScale(wscale);

		m_pItem = pItem;
	}
}
