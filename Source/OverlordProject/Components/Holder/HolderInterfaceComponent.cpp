#include "stdafx.h"
#include "HolderInterfaceComponent.h"
#include "ModelComponent.h"
#include "HolderComponent.h"
#include "../Food/FoodComponent.h"
#include "HolderIconsComponent.h"
#include "HolderProgressComponent.h"
#include "GameScene.h"
#include "../Food/FoodInterfaceComponent.h"
#include "ParticleEmitterComponent.h"
#include "../../Singletons/AssetFiles.h"
#include "TransformComponent.h"
#include "../../Helpers/Events.h"
#include "../../Materials/ColorMaterial.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"

HolderInterfaceComponent::HolderInterfaceComponent(const HolderInfo& info)
	: m_HolderInfo(info)
	, m_pMaterial(nullptr)
	, m_pHolderComponent(nullptr)
	, m_IsHighlighted(false)
	, m_pProgressComponent(nullptr)
	, m_pIconsComponent(nullptr)
	, m_IsBeingConverted(false)
	, m_pParticleComponent(nullptr)
	, m_IsBurned(false)
{
}
PrototypeComponent* HolderInterfaceComponent::Clone() const
{
	return new HolderInterfaceComponent(m_HolderInfo);
}

void HolderInterfaceComponent::AsClone()
{
	auto* pObject = GetGameObject();
	if (pObject)
	{
		//HolderComponent
		//***************
		m_pHolderComponent = pObject->GetDynamicComponent<HolderComponent>();

		//IconsComponent
		//**************
		m_pIconsComponent = pObject->GetDynamicComponent<HolderIconsComponent>();

		//ProgressComponent
		//*****************
		m_pProgressComponent = pObject->GetDynamicComponent<HolderProgressComponent>();
	}
}


//
void HolderInterfaceComponent::Highlight(const DirectX::XMFLOAT4& color)
{
	if (!m_IsHighlighted)
	{
		m_IsHighlighted = true;
		HighlightEnable(color);
	}
}

//Food
bool HolderInterfaceComponent::AddFood(HolderInterfaceComponent& other)
{
	return (m_pHolderComponent && other.m_pHolderComponent && m_pHolderComponent->AddFood(*other.m_pHolderComponent));
}
bool HolderInterfaceComponent::AddFood(FoodInterfaceComponent& food)
{
	return (m_pHolderComponent && m_pHolderComponent->AddFood(&food));
}
FoodInterfaceComponent* HolderInterfaceComponent::RemoveFood()
{
	if (m_pHolderComponent)
	{
		return m_pHolderComponent->RemoveFood();
	}
	return nullptr;
}

void HolderInterfaceComponent::Burn()
{
	if (m_pHolderComponent)
	{
		m_pHolderComponent->BurnFood();
	}
}


//Converting
eConvertReturn HolderInterfaceComponent::Convert(float elapsed, eUpdate update, BoxComponentBase* pBox)
{
	m_IsBeingConverted = true;
	if (m_pHolderComponent)
	{
		//Convert Food
		//************
		return m_pHolderComponent->Convert(elapsed, update, pBox);
	}
	return eConvertReturn::Failed;
}


//Base
void HolderInterfaceComponent::Initialize(const GameContext& gameContext)
{
	//Components
	//**********
	GameObject* pObject = GetGameObject();

	//ModelComponent
	//**************
	auto modelpair = m_HolderInfo.modelInfo.MakeDiffuseModel(gameContext);
	m_pMaterial = modelpair.second.first;
	auto* pModelComponent = modelpair.first;
	pObject->AddComponent(pModelComponent);

	//ParticleEmitter
	//***************
	std::wstring asset{};
	if (m_HolderInfo.canBurn && AssetFiles::GetInstance().Get(eAssetId::Particle_Smoke, asset))
	{
		m_pParticleComponent = new ParticleEmitterComponent(asset);

		m_pParticleComponent->SetColor({ 1,1,1,1 });
		m_pParticleComponent->SetMaxEmitterRange(0.0f);
		m_pParticleComponent->SetMinEmitterRange(0.0f);
		m_pParticleComponent->SetVelocity({ 0, 2.5f, 0 });
		m_pParticleComponent->SetMinSize(1.0f);
		m_pParticleComponent->SetMaxSize(1.0f);
		m_pParticleComponent->SetMaxEnergy(3);
		m_pParticleComponent->SetMinEnergy(1.0f);
		m_pParticleComponent->SetMaxSizeGrow(2.5f);
		m_pParticleComponent->SetMinSizeGrow(0.0f);
		m_pParticleComponent->SetActiveDraw(true);
		m_pParticleComponent->SetActiveUpdate(true);
		m_pParticleComponent->SetActiveSpawn(false);

		pObject->AddComponent(m_pParticleComponent);
	}

	//HolderComponent
	//***************
	m_pHolderComponent = pObject->GetDynamicComponent<HolderComponent>();

	//IconsComponent
	//**************
	m_pIconsComponent = pObject->GetDynamicComponent<HolderIconsComponent>();
	
	//ProgressComponent
	//*****************
	m_pProgressComponent = pObject->GetDynamicComponent<HolderProgressComponent>();

	
}
void HolderInterfaceComponent::Update(const GameContext& gameContext)
{
	//Update Progress Timer & States (& Smoke Particles)
	//**************************************************
	if (m_pProgressComponent && m_pHolderComponent)
	{
		//Calculate totalAccu, totalTarget & minimumOvertime
		//**************************************************
		const int size = m_pHolderComponent->GetIndex();
		float totalAccu = 0.0f;
		float totalTarget = 0.0f;
		float minOvertime = (size > 0) ? (std::numeric_limits<float>::max()) : (0.0f);
		
		for (int idx = 0; idx < size; ++idx)
		{
			const FoodPointer& foodPointer = m_pHolderComponent->GetFoodPointer(idx);
			FoodInterfaceComponent* pFoodInterface = foodPointer.Get();
			FoodComponent* pFoodComponent = pFoodInterface->GetFoodComponent();

			if (!m_IsBeingConverted) pFoodComponent->ReduceOvertime(gameContext.pGameTime->GetElapsed());

			//Extract Values
			//**************
			float accu = pFoodComponent->GetConversionAccu();
			float target = pFoodComponent->GetConversionTime();
			totalTarget += target;
			totalAccu += (accu < target) ? accu : target;
			minOvertime = std::min(minOvertime, accu - target);
		}

		//Update
		//******
		m_pProgressComponent->UpdateTime(totalAccu, totalTarget);
		const float overtimeScale = OvertimeScale(minOvertime);

		if (!(m_IsBurned && overtimeScale > FLT_EPSILON))
		{
			m_pProgressComponent->UpdateState(overtimeScale);
			bool wasBurned = m_IsBurned;
			m_IsBurned = overtimeScale >= 1.0f;
			if (m_IsBurned && !wasBurned)
			{
				GetGameObject()->Notify(Events::EVENT_HOLDER_FOOD_BURNED);
			}
			else if (!m_IsBurned && wasBurned)
			{
				GetGameObject()->Notify(Events::EVENT_HOLDER_FOOD_UNBURNED);
			}
		}	

		//Update Burned Particles
		//***********************
		if (m_pParticleComponent)
		{
			float alpha = overtimeScale;
			Clamp(alpha, 1.0f, 0.0f);

			m_pParticleComponent->SetActiveSpawn(alpha > FLT_EPSILON);
			m_pParticleComponent->SetColor({ 1,1,1, alpha });
		}
	}
	m_IsBeingConverted = false;
}
void HolderInterfaceComponent::Draw(const GameContext&)
{
}
void HolderInterfaceComponent::PostDraw(const GameContext&)
{
	if (m_IsHighlighted)
	{
		m_IsHighlighted = false;
		HighlightDisable();
	}
}



//Helpers
void HolderInterfaceComponent::HighlightEnable(const DirectX::XMFLOAT4& color)
{
	if (m_pMaterial)
	{
		m_pMaterial->SetColor(color);
	}
}
void HolderInterfaceComponent::HighlightDisable()
{
	if (m_pMaterial)
	{
		m_pMaterial->SetColor({ 1,1,1,1 });
	}
}

float HolderInterfaceComponent::OvertimeScale(float minOvertime) const
{
	//Return number
	//	> 1.0f => No overtime
	//	< 0.0f => Not Finished
	//	< 1.0f && > 0.0f => Finished

	const float overtimeTime = 7.5f;
	return minOvertime / overtimeTime;
}
