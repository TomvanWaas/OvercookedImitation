#include "stdafx.h"
#include "ColorFlickerComponent.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "GameScene.h"
#include "Destroyer.h"

ColorFlickerComponent::ColorFlickerComponent(const DirectX::XMFLOAT4& colorA, const DirectX::XMFLOAT4& colorB, float speed,
	std::vector<SpriteComponent*> pSprites)
	: m_Accu(0)
	, m_Time(-1)
	, m_Speed(speed)
	, m_pSprites(pSprites)
	, m_ColorA(colorA)
	, m_ColorB(colorB)
{
}


void ColorFlickerComponent::Enable(bool enable, float time)
{
	m_Time = time;
	m_IsEnabled = enable;
	m_Accu = 0;
	if (enable ==  false)
	{
		for (auto* pSprite : m_pSprites)
		{
			if (pSprite) pSprite->SetColor({ 1,1,1,1 });
		}
	}
}

void ColorFlickerComponent::Initialize(const GameContext&)
{
	//Empty => Init by Get All
	if (m_pSprites.size() == 0)
	{
		m_pSprites = GetGameObject()->GetComponents<SpriteComponent>(true);
	}
}

void ColorFlickerComponent::Update(const GameContext& gameContext)
{
	if (m_IsEnabled)
	{
		m_Accu += gameContext.pGameTime->GetElapsed();
		float sin = sinf(m_Accu * m_Speed);	//[-1, 1]
		sin = (sin + 1) * 0.5f;				//[ 0, 1]
		for (SpriteComponent* pSprite : m_pSprites)
		{
			if (pSprite) pSprite->SetColor(Lerp(m_ColorA, m_ColorB, sin));
		}
		if (m_Accu >= m_Time && m_Time > 0)
		{
			if (m_OnEnd) m_OnEnd();
			Enable(false);
		}
	}
}

void ColorFlickerComponent::Draw(const GameContext&)
{
}

DirectX::XMFLOAT4 ColorFlickerComponent::Lerp(DirectX::XMFLOAT4 a, DirectX::XMFLOAT4 b, float t) const
{
	return { a.x * (1 - t) + b.x*t,
			a.y * (1 - t) + b.y*t,
			a.z * (1 - t) + b.z*t,
			a.w * (1 - t) + b.w*t };
}
