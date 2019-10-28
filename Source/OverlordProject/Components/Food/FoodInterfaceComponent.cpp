#include "stdafx.h"
#include "FoodInterfaceComponent.h"
#include "ModelComponent.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "FoodComponent.h"
#include "Observed.h"
#include "../../Helpers/Events.h"
#include "GameScene.h"
#include "Destroyer.h"

FoodInterfaceComponent::FoodInterfaceComponent(const FoodInfo& info)
	: m_FoodInfo(info)
	, m_pFoodComponent(nullptr)
	, m_pMaterial(nullptr)
	, m_IsHighlighted(false)
{
}
PrototypeComponent* FoodInterfaceComponent::Clone() const
{
	return new FoodInterfaceComponent(m_FoodInfo);
}

void FoodInterfaceComponent::AsClone()
{
	auto* pObject = GetGameObject();
	if (pObject)
	{
		m_pFoodComponent = pObject->GetDynamicComponent<FoodComponent>();
	}
}


//
void FoodInterfaceComponent::Highlight(const DirectX::XMFLOAT4& color)
{
	if (!m_IsHighlighted)
	{
		m_IsHighlighted = true;
		HighlightEnable(color);
	}
}


//Converting
eConvertReturn FoodInterfaceComponent::Convert(eFoodState state)
{
	if (m_pFoodComponent)
	{
		return m_pFoodComponent->Convert(state);
	}
	return eConvertReturn::Failed;
}
eConvertReturn FoodInterfaceComponent::Convert(float elapsed, eUpdate update, HolderComponent* pHolder, FoodConversion& conversion)
{
	if (m_pFoodComponent)
	{
		return m_pFoodComponent->Convert(elapsed, update, pHolder, conversion);
	}
	return eConvertReturn::Failed;
}
bool FoodInterfaceComponent::CanConvert(eUpdate update) const
{
	if (m_pFoodComponent)
	{
		return m_pFoodComponent->CanConvert(update);
	}
	return false;
}

//State & Type
eFoodState FoodInterfaceComponent::GetState() const
{
	if (m_pFoodComponent)
	{
		return m_pFoodComponent->GetState();
	}
	return eFoodState::None;
}
eFoodType FoodInterfaceComponent::GetType() const
{
	return m_FoodInfo.type;
}


//Base
void FoodInterfaceComponent::OnNotify(ObservedID observedID)
{
	switch (observedID.id)
	{
	case Events::EVENT_FOOD_STATE_CHANGED:
	{
		GameObject* pObject = GetGameObject();
		GameScene* pScene = pObject->GetScene();
		if (m_pFoodComponent && pScene)
		{
			SwapModel(m_pFoodComponent->GetState(), pScene->GetGameContext());
		}
	}
	break;

	//...
	}

	auto* pParent = GetGameObject()->GetParent();
	if (pParent) pParent->Notify(observedID);
}
void FoodInterfaceComponent::Initialize(const GameContext&)
{
	//Components
	//**********
	GameObject* pObject = GetGameObject();

	//FoodComponent
	//*************
	m_pFoodComponent = pObject->GetDynamicComponent<FoodComponent>();


	//...

}
void FoodInterfaceComponent::Update(const GameContext&)
{
}
void FoodInterfaceComponent::Draw(const GameContext&)
{
}
void FoodInterfaceComponent::PostDraw(const GameContext&)
{
	if (m_IsHighlighted)
	{
		m_IsHighlighted = false;
		HighlightDisable();
	}
}


//Helpers
void FoodInterfaceComponent::HighlightEnable(const DirectX::XMFLOAT4& color)
{
	if (m_pMaterial)
	{
		m_pMaterial->SetColor(color);
	}
}
void FoodInterfaceComponent::HighlightDisable()
{
	if (m_pMaterial)
	{
		m_pMaterial->SetColor({ 1,1,1,1 });
	}
}
void FoodInterfaceComponent::SwapModel(eFoodState state, const GameContext& gameContext)
{
	GameObject* pObject = GetGameObject();

	//Find New Model
	//**************
	auto it = m_FoodInfo.models.find(state);
	if (it != m_FoodInfo.models.end())
	{
		auto* pMatManager = gameContext.pMaterialManager;
		auto modelpair = it->second.MakeDiffuseModel(gameContext);

		//New Data
		//********
		ModelComponent* pNewModel = modelpair.first;
		ColorMaterial* pNewMaterial = modelpair.second.first;
		UINT newMaterialID = modelpair.second.second;
		bool validNewMaterial = bool(pNewMaterial);

		//Old Data
		//********
		ModelComponent* pOldModel = pObject->GetComponent<ModelComponent>();
		UINT oldMaterialID = (pOldModel) ? pOldModel->GetMaterialID() : 0;
		bool validOldMaterial = bool(pOldModel);

		//Exchange
		//********

		//New Model
		if (pNewModel)
		{
			//Model
			if (pOldModel) {
				pObject->RemoveComponent(pOldModel);
				Destroyer::GetInstance().Destroy(pOldModel);
				pOldModel = nullptr;
			}
			pObject->AddComponent(pNewModel);

			//Material
			if (validNewMaterial && (!validOldMaterial || pNewMaterial != pMatManager->GetMaterial(oldMaterialID)))
			{
				pNewModel->SetMaterial(newMaterialID);
				m_pMaterial = pNewMaterial;
				if (validOldMaterial)
				{
					pMatManager->RemoveMaterial(oldMaterialID);
				}
			}
			else if (validOldMaterial)
			{
				pNewModel->SetMaterial(oldMaterialID);
			}
		}

		//Old Model
		else if (pOldModel)
		{
			if (validNewMaterial)
			{
				pOldModel->SetMaterial(newMaterialID);
				m_pMaterial = pNewMaterial;
				if (validOldMaterial)
				{
					pMatManager->RemoveMaterial(oldMaterialID);
				}
			}
			//else if (validOldMaterial)
			//{
			//	//Current state => Do Nothing	
			//}
		}
	}
#ifdef _DEBUG
	else
	{
		Logger::LogWarning(L"FoodInterfaceComponent::SwapModel >> Invalid FoodState");
	}
#endif
}
