#include "stdafx.h"
#include "FoodPointer.h"
#include "GameObject.h"
#include "GameScene.h"
#include "TransformComponent.h"
#include "FoodInterfaceComponent.h"

FoodInterfaceComponent* FoodPointer::Get() const
{
	return m_pFoodObject;
}
GameObject* FoodPointer::GetParent() const
{
	return m_pParent;
}

FoodInterfaceComponent* FoodPointer::Set(FoodInterfaceComponent* pFood)
{
	FoodInterfaceComponent* pPrevious = m_pFoodObject;
	HelperRemove();
	HelperAdd(pFood);
	return pPrevious;
}
FoodInterfaceComponent* FoodPointer::Remove()
{
	FoodInterfaceComponent* pPrevious = m_pFoodObject;
	HelperRemove();
	return pPrevious;
}
void FoodPointer::Swap(FoodPointer& other)
{
	FoodInterfaceComponent* pPreviousSelf = m_pFoodObject;
	FoodInterfaceComponent* pPreviousOther = other.m_pFoodObject;
	HelperRemove();
	other.HelperRemove();
	HelperAdd(pPreviousOther);
	other.HelperAdd(pPreviousSelf);
}

void FoodPointer::Initialize(GameObject* pParent, const DirectX::XMFLOAT3& foodOffset)
{
	m_pParent = pParent;
	m_Offset = foodOffset;
}

bool FoodPointer::Initialized() const
{
	return bool(m_pParent);
}
bool FoodPointer::IsValid() const
{
	return bool(m_pFoodObject);
}





void FoodPointer::HelperRemove()
{
	if (m_pFoodObject && m_pParent)
	{
		m_pParent->RemoveChild(m_pFoodObject->GetGameObject());
		m_pFoodObject = nullptr;
	}
}
void FoodPointer::HelperAdd(FoodInterfaceComponent* pFood)
{
	if (pFood && m_pParent)
	{
		GameObject* pObject = pFood->GetGameObject();

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
		pObject->GetTransform()->Translate(m_Offset);

		//Random Rotation Y
		pObject->GetTransform()->Rotate(0, randF(0.0f, 360.0f), 0, true);

		m_pFoodObject = pFood;
	}
}
