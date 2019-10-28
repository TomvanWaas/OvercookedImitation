#include "stdafx.h"
#include "HolderParticleComponent.h"
#include "ParticleEmitterComponent.h"
#include "../../Singletons/AssetFiles.h"
#include "GameObject.h"

HolderParticleComponent::HolderParticleComponent(eAssetId particleID, const DirectX::XMFLOAT4& color)
	: m_ParticleAsset(particleID)
	, m_Color(color)
	, m_pParticleComponent(nullptr)
	, m_Speed(1.0f)
	, m_Accu(0.0f)
	, m_State(eState::Not)
{
}
PrototypeComponent* HolderParticleComponent::Clone() const
{
	return new HolderParticleComponent(m_ParticleAsset, m_Color);
}

void HolderParticleComponent::Activate(float fadeTime)
{
	m_Speed = 1 / fadeTime;
	m_State = eState::Up;
	if (m_pParticleComponent)
	{
		m_pParticleComponent->SetActiveDraw(true);
		m_pParticleComponent->SetActiveUpdate(true);
	}
}
void HolderParticleComponent::Deactivate(float fadeTime)
{
	m_Speed = 1 / fadeTime;
	m_State = eState::Down;
	if (m_pParticleComponent)
	{
		m_pParticleComponent->SetActiveDraw(true);
		m_pParticleComponent->SetActiveUpdate(true);
	}
}



void HolderParticleComponent::Initialize(const GameContext&)
{
	GameObject* pObject = GetGameObject();

	//ParticleEmitter
	//***************
	std::wstring asset{};
	if (AssetFiles::GetInstance().Get(m_ParticleAsset, asset))
	{
		m_pParticleComponent = new ParticleEmitterComponent(asset, 50);

		m_pParticleComponent->SetColor(m_Color);
		m_pParticleComponent->SetMaxEmitterRange(0.0f);
		m_pParticleComponent->SetMinEmitterRange(0.0f);
		m_pParticleComponent->SetVelocity({ 0, 2.5f, 0 });
		m_pParticleComponent->SetMinSize(1.0f);
		m_pParticleComponent->SetMaxSize(1.0f);
		m_pParticleComponent->SetMaxEnergy(3);
		m_pParticleComponent->SetMinEnergy(1.0f);
		m_pParticleComponent->SetMaxSizeGrow(2.5f);
		m_pParticleComponent->SetMinSizeGrow(0.0f);

		bool isActive = false;
		m_pParticleComponent->SetActiveDraw(isActive);
		m_pParticleComponent->SetActiveUpdate(isActive);

		pObject->AddComponent(m_pParticleComponent);
	}



}
void HolderParticleComponent::Update(const GameContext& gameContext)
{
	if (!m_pParticleComponent) return;

	switch (m_State)
	{
	case eState::Not:
		//Inactive
		//********
		return;
		break;

	case eState::Up:
	{
		//Calculate Alpha
		//***************
		m_Accu += gameContext.pGameTime->GetElapsed() * m_Speed;
		if (m_Accu >= 1.0f)
		{
			m_Accu = 1.0f;
			m_State = eState::Not;
		}
	}
	break;

	case eState::Down:
	{
		//Calculate Alpha
		//***************
		m_Accu -= gameContext.pGameTime->GetElapsed() * m_Speed;
		if (m_Accu <= 0.0f)
		{
			m_Accu = 0.0f;
			m_State = eState::Not;
			if (m_pParticleComponent)
			{
				m_pParticleComponent->SetActiveDraw(false);
				m_pParticleComponent->SetActiveUpdate(false);
			}
		}
	}
	break;
	}

	//Set Alpha
	//*********
	float alpha = m_Accu * m_Color.w;
	DirectX::XMFLOAT4 color = { m_Color.x, m_Color.y, m_Color.z, alpha };
	m_pParticleComponent->SetColor(color);

}
void HolderParticleComponent::Draw(const GameContext&)
{
}

