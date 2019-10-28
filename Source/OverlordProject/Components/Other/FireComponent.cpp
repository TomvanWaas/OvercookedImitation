#include "stdafx.h"
#include "FireComponent.h"
#include "GameObject.h"
#include "GameScene.h"
#include "TransformComponent.h"
#include "ParticleEmitterComponent.h"
#include "Observed.h"
#include "../../Helpers/Events.h"
#include "../../Components/Visuals/PostProcessingManagerComponent.h"
#include "SoundManager.h"
#include "../../Helpers/SoundID.h"

FireComponent::FireComponent(const std::wstring& assetFile, float spreadDelay, float spreadDistance,
	float extinguishDuration)
	: m_AssetFile(assetFile)
	, m_SpreadDelay(spreadDelay)
	, m_SpreadDistance(spreadDistance)
	, m_ExtinguishDuration(extinguishDuration)
	, m_SpreadAccu(0.0f)
	, m_ExtinguishAccu(0.0f)
	, m_IsLit(false)
	, m_HasSpread(false)
	, m_pParticleEmitter(nullptr)
{
}

void FireComponent::Extinguish(float elapsed)
{
	if (m_IsLit)
	{
		m_ExtinguishAccu += elapsed;
		if (m_ExtinguishAccu >= m_ExtinguishDuration)
		{
			m_ExtinguishAccu = m_ExtinguishDuration;

			m_IsLit = false;

			//Notify
			GetGameObject()->Notify(Events::EVENT_BOX_FIRE_ENDED);

			//Post Processing
			CheckIntensity();
		}
	}
}
void FireComponent::Lit()
{
	if (!m_IsLit)
	{
		m_IsLit = true;

		//Notify
		GetGameObject()->Notify(Events::EVENT_BOX_FIRE_STARTED);

		//Reset Values
		m_SpreadAccu = 0.0f;
		m_ExtinguishAccu = 0.0f;
		m_HasSpread = false;

		//Post Processing Effect
		CheckIntensity();
	}
}

void FireComponent::Initialize(const GameContext&)
{
	GameObject* pObject = GetGameObject();

	//Particle Emitter
	//****************
	m_pParticleEmitter = new ParticleEmitterComponent(m_AssetFile, 50);
	m_pParticleEmitter->SetColor({1, 1, 1, 0});
	m_pParticleEmitter->SetMaxEmitterRange(1);
	m_pParticleEmitter->SetMinEmitterRange(0.0f);
	m_pParticleEmitter->SetVelocity({ 0, 7.5f, 0 });
	m_pParticleEmitter->SetMinSize(2.5f);
	m_pParticleEmitter->SetMaxSize(3.5f);
	m_pParticleEmitter->SetMaxEnergy(1);
	m_pParticleEmitter->SetMinEnergy(0.25f);
	m_pParticleEmitter->SetMaxSizeGrow(2.5f);
	m_pParticleEmitter->SetMinSizeGrow(0.0f);
	m_pParticleEmitter->SetActiveSpawn(false);
	pObject->AddComponent(m_pParticleEmitter);
}
void FireComponent::Update(const GameContext& gameContext)
{
	//Update Spreading
	//****************
	if (m_IsLit && !m_HasSpread)
	{
		m_SpreadAccu += gameContext.pGameTime->GetElapsed();
		if (m_SpreadAccu >= m_SpreadDelay)
		{
			m_HasSpread = true;

			//Spread
			GameScene* pScene = GetGameObject()->GetScene();
			std::vector<FireComponent*> pFireComponents = pScene->GetDynamicObjectsWith<FireComponent>(true);
			for (size_t i = 0, j = pFireComponents.size(); i < j; ++i)
			{
				FireComponent* pFire = pFireComponents[i];
				if (pFire && pFire != this && InDistance(*pFire))
				{
					pFire->Lit();
				}
			}
		}
	}

	//Update Particle Emitter
	//***********************
	if (m_pParticleEmitter)
	{
		//Color
		DirectX::XMFLOAT4 color = m_pParticleEmitter->GetSettings().Color;
		const float elapsed = gameContext.pGameTime->GetElapsed();

		const float target = float(1 - 0.5f*(m_ExtinguishAccu / m_ExtinguishDuration)) * int(m_IsLit);
		color.w = color.w * (1 - elapsed) + elapsed * target;
		m_pParticleEmitter->SetColor(color);

		//Spawn
		m_pParticleEmitter->SetActiveSpawn(m_IsLit);
		bool active = color.w > FLT_EPSILON;
		m_pParticleEmitter->SetActiveDraw(active);
		m_pParticleEmitter->SetActiveUpdate(active);
	}
}
void FireComponent::OnNotify(ObservedID observedID)
{
	switch(observedID.id)
	{
	case Events::EVENT_HOLDER_FOOD_BURNED:
		Lit();
		break;
	}
}


bool FireComponent::InDistance(FireComponent& pFire)
{
	const DirectX::XMFLOAT3& a = GetTransform()->GetWorldPosition();
	const DirectX::XMFLOAT3& b = pFire.GetTransform()->GetWorldPosition();
	return ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z)) <= m_SpreadDistance*m_SpreadDistance;
}

void FireComponent::CheckIntensity() const
{
	auto* pScene = GetGameObject()->GetScene();

	//Get All FireComponents in Scene
	//*******************************
	std::vector<FireComponent*> pObjects = pScene->GetDynamicObjectsWith<FireComponent>(true);

	//Count Lit Fires
	//***************
	int litCount = 0;
	for (size_t i = 0, j = pObjects.size(); i < j; ++i)
	{
		litCount += int(pObjects[i]->IsLit());
	}
	const float litFraction = (pObjects.size() > 0) ? (litCount / float(pObjects.size())) : 0.0f;

	//Fire Post Processing Effect
	//***************************
	auto* pFireManager = pScene->GetDynamicObjectWith<PostProcessingManagerComponent>(true);
	if (pFireManager)
	{
		const float maxIntensity = 5.0f;
		const float minIntensity = 1.5f;
		const float speed = 0.5f;

		//Calc Intensity
		//**************
		const float intensity = (litCount > 0) ? (litFraction * (maxIntensity - minIntensity) + minIntensity) : 0.0f;
		pFireManager->SetIntensity(intensity, speed);
	}

	//Sound Effect
	//************
	auto* pSoundManager = SoundManager::GetInstance();
	{
		const float maxVolume = 0.1f;
		const float minVolume = 0.05f;

		const int soundID = int(eSoundID::FireLooped);
		const int channelID = int(eChannelID::Looped01);

		//Calc Volume
		//***********
		if (litCount > 0)
		{
			const float volume = (litFraction * (maxVolume - minVolume) + minVolume);
			pSoundManager->PlaySoundByID(soundID, channelID, volume, eSoundMerge::Replay);
		}
		else
		{
			pSoundManager->StopIf(channelID, soundID);
		}
	}
}
