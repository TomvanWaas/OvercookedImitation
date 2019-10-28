#include "stdafx.h"
#include "PostProcessingManagerComponent.h"
#include "../../Materials/PostProcessing/FirePostMaterial.h"



PostProcessingManagerComponent::PostProcessingManagerComponent(FirePostMaterial* pFirePostMaterial)
	: m_pFirePostMaterial(pFirePostMaterial)
	, m_Speed(0)
	, m_Target(0)
	, m_IsActive(false)
{
	if (m_pFirePostMaterial)
	{
		m_pFirePostMaterial->SetIntensity(m_Target);
	}
}

void PostProcessingManagerComponent::SetIntensity(float intensity, float speed)
{
	m_Target = intensity;
	m_Speed = speed;
	m_IsActive = true;
}


void PostProcessingManagerComponent::Update(const GameContext& gameContext)
{
	if (!m_IsActive || !m_pFirePostMaterial) return;

	const float elapsed = gameContext.pGameTime->GetElapsed();
	float intensity = m_pFirePostMaterial->GetIntensity();

	if (m_Target < intensity)
	{
		intensity -= elapsed * m_Speed;
		if (m_Target >= intensity)
		{
			intensity = m_Target;
			m_IsActive = false;
		}
	}
	else
	{
		intensity += elapsed * m_Speed;
		if (m_Target <= intensity)
		{
			intensity = m_Target;
			m_IsActive = false;
		}
	}

	m_pFirePostMaterial->SetIntensity(intensity);	
}


