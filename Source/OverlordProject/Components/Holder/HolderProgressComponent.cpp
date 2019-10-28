#include "stdafx.h"
#include "HolderProgressComponent.h"
#include "../../Singletons/AssetFiles.h"
#include "../../Components/Visuals/ParticleComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "../../Components/Visuals/FadeParticleComponent.h"
#include "../../Components/Visuals/SliderParticleComponent.h"
#include "SoundManager.h"
#include "../../Helpers/SoundID.h"

HolderProgressComponent::HolderProgressComponent(const HolderInfo& info)
	: m_HolderInfo(info)
	, m_State(eProgressState::Ready)
	, m_pStateParticle(nullptr)
	, m_pBackParticle(nullptr)
	, m_pTimerSlider(nullptr)
{
}

PrototypeComponent* HolderProgressComponent::Clone() const
{
	return new HolderProgressComponent(m_HolderInfo);
}

void HolderProgressComponent::UpdateTime(float totalAccu, float totalTarget)
{
	const float fraction = (totalAccu != 0.0f) ? (totalAccu / totalTarget) : 0.0f;

	//Show Or Don't Show? (Timer & Background)
	//****************************************
	if (ShowTimer(fraction))
	{
		//Show
		//****
		if (m_pTimerSlider)
		{
			m_pTimerSlider->SetSliderLength(fraction);
		}
		if (m_pBackParticle)
		{
			m_pBackParticle->SetSize({ 1,1 });
		}
	}
	else
	{
		//Don't Show
		//**********
		if (m_pTimerSlider)
		{
			m_pTimerSlider->SetSliderLength(0.0f);
		}
		if (m_pBackParticle)
		{
			m_pBackParticle->SetSize({ 0,0 });
		}
	}
}
void HolderProgressComponent::UpdateState(float overtimeScale)
{
	if (!m_pStateParticle) return;
	const float overtime = 0.5f;

	//Update State
	//************
	//- Active	: ]-, 0[
	//- Ready	: [0, overtime[
	//- Overtime: [overtime, +[

	//Active
	//******
	if (overtimeScale <= 0 || overtimeScale >= 1.0f)
	{
		SetState(eProgressState::Active);
	}
	//Overtime
	//********
	else if (overtimeScale >= overtime)
	{
		SetState(eProgressState::Overtime);
	}
	//Ready
	//*****
	else
	{
		SetState(eProgressState::Ready);
	}


	//Execute State
	//*************
	switch (m_State)
	{
	//case eProgressState::Active:
	//	//Nothing
	//	break;
	//case eProgressState::Ready:
	//	//Nothing
	//	break;
	case eProgressState::Overtime:
		//Update FadeSpeed
		//****************
		float t = (overtimeScale - overtime) / (1 - overtime);
		//Lerp
		const float maxSpeed = 6.0f;
		const float minSpeed = 4.0f;
		m_pStateParticle->SetFadeSpeed(minSpeed * (1 - t) + maxSpeed * t);
		break;
	}


}


void HolderProgressComponent::Initialize(const GameContext&)
{
	//Billboards
	//**********
	GameObject* pObject = GetGameObject();
	GameObject* pChild = new GameObject();
	std::wstring asset{};

	const float scale = 0.0175f;
	
	//Timer
	//*****
	if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Progress_Timer, asset))
	{
		auto* pTimerChild = new GameObject();
		m_pTimerSlider = new SliderParticleComponent(asset);
		pTimerChild->AddComponent(m_pTimerSlider);
		m_pTimerSlider->SetOffset(m_HolderInfo.stateOffsetScreen);
		m_pTimerSlider->SetSliderLength(0.0f);
		pChild->AddChild(pTimerChild);

	}

	//Background
	//**********
	if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Progress_Background, asset))
	{
		m_pBackParticle = new ParticleComponent(asset);
		m_pBackParticle->SetSize({ 0,0 });
		m_pBackParticle->SetOffset(m_HolderInfo.stateOffsetScreen);
		pChild->AddComponent(m_pBackParticle);
	}

	//State
	//*****
	if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Progress_States, asset))
	{
		m_pStateParticle = new FadeParticleComponent(asset, 1.0f);
		m_pStateParticle->FadeOut();
		m_pStateParticle->SetOffset(m_HolderInfo.stateOffsetScreen);
		pChild->AddComponent(m_pStateParticle);
	}

	pChild->GetTransform()->Scale(scale, scale, scale);
	pObject->AddChild(pChild);
}
void HolderProgressComponent::Update(const GameContext&)
{
}
void HolderProgressComponent::Draw(const GameContext&)
{
}

bool HolderProgressComponent::ShowTimer(float fraction)
{
	return (fraction > FLT_EPSILON && fraction < (1 - FLT_EPSILON));
}
void HolderProgressComponent::SetState(eProgressState state)
{
	if (state == m_State) return;
	switch (state)
	{
	case eProgressState::Active:
	{
		m_pStateParticle->SetFadeSpeed(3.0f);
		m_pStateParticle->FadeOut();
		m_pStateParticle->SetOnFadeEnd(nullptr);
	}
	break;
	case eProgressState::Ready:
	{
			//From Active
		if (m_State == eProgressState::Active)
		{
			m_pStateParticle->SetFadeSpeed(1.5f);
			m_pStateParticle->FadeIn();
			m_pStateParticle->SetOnFadeEnd([this](FadeParticleComponent::eFadeState)
			{
				m_pStateParticle->FadeOut();
				m_pStateParticle->SetOnFadeEnd(nullptr);
			});
			m_pStateParticle->SetSize({ 0.5f, 1 });
			m_pStateParticle->SetPivot({ 0,0 });
		}
			//From Overtime
		else
		{
			m_pStateParticle->SetFadeSpeed(3.0f);
			m_pStateParticle->FadeOut();
			m_pStateParticle->SetOnFadeEnd(nullptr);
		}
	}
	break;
	case eProgressState::Overtime:
	{
		m_pStateParticle->SetFadeSpeed(1.5f);
		m_pStateParticle->FadeIn();
		m_pStateParticle->SetOnFadeEnd([this](FadeParticleComponent::eFadeState prevState)
		{
			switch (prevState)
			{
			case FadeParticleComponent::eFadeState::In:
				m_pStateParticle->FadeOut();
				//Sound
				//*****
				SoundManager::GetInstance()->PlaySoundByID(int(eSoundID::AlarmSingle), int(eChannelID::Effect01), 0.005f, eSoundMerge::Overlap);
				break;
			case FadeParticleComponent::eFadeState::Out:
				m_pStateParticle->FadeIn();
				break;
			}
		});
		m_pStateParticle->SetSize({ 0.5f, 1 });
		m_pStateParticle->SetPivot({ 0.5f,0 });
	}
	break;
	}
	m_State = state;
}
