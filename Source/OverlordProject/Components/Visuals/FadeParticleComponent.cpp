#include "stdafx.h"
#include "FadeParticleComponent.h"
#include "GameObject.h"

FadeParticleComponent::FadeParticleComponent(const std::wstring& assetFile, float speed, const std::function<void(eFadeState)>& onEnd)
	: ParticleComponent(assetFile)
	, m_FadeSpeed(speed)
	, m_FadeState(eFadeState::None)
	, m_OnFadeEnd(onEnd)
{
}

void FadeParticleComponent::SetFadeInactive()
{
	m_FadeState = eFadeState::None;
	auto color = GetColor();
	SetColor({ color.x,color.y,color.z, 0 });
}

void FadeParticleComponent::FadeIn()
{
	m_FadeState = eFadeState::In;
}
void FadeParticleComponent::FadeOut()
{
	m_FadeState = eFadeState::Out;
}


void FadeParticleComponent::Initialize(const GameContext& gameContext)
{
	ParticleComponent::Initialize(gameContext);

	switch (m_FadeState)
	{
	case eFadeState::None:
		break;
	case eFadeState::In:
	{
		DirectX::XMFLOAT4 color = GetColor();
		SetColor({ color.x, color.y, color.z, 1.0f });
	}
	break;
	case eFadeState::Out:
	{
		DirectX::XMFLOAT4 color = GetColor();
		SetColor({ color.x, color.y, color.z, 0.0f });
	}
	break;
	}
}
void FadeParticleComponent::Update(const GameContext& gameContext)
{
	ParticleComponent::Update(gameContext);

	if (m_FadeState == eFadeState::None) return;

	DirectX::XMFLOAT4 color = GetColor();
	float delta = gameContext.pGameTime->GetElapsed() * m_FadeSpeed;
	switch(m_FadeState)
	{
	case eFadeState::Out:
		color.w -= delta;
		if (color.w <= 0.0f)
		{
			color.w = 0.0f;
			m_FadeState = eFadeState::None;
			if (m_OnFadeEnd) m_OnFadeEnd(eFadeState::Out);
		}
		break;
	case eFadeState::In:
		color.w += delta;
		if (color.w >= 1.0f)
		{
			color.w = 1.0f;
			m_FadeState = eFadeState::None;
			if (m_OnFadeEnd) m_OnFadeEnd(eFadeState::In);
		}
		break;
	}

	SetColor(color);
}