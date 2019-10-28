#include "stdafx.h"
#include "ScoreTextComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "GameScene.h"
#include "Destroyer.h"

ScoreTextComponent::ScoreTextComponent(int score, const std::wstring& font, const DirectX::XMFLOAT4& color,
	const DirectX::XMFLOAT3& veloc, const DirectX::XMFLOAT3& decay, float alphaSpeed)
	: m_Score(score)
	, m_Font(font)
	, m_Color(color)
	, m_Velocity(veloc)
	, m_Decay(decay)
	, m_AlphaSpeed(alphaSpeed)
	, m_pTextComponent(nullptr)
{
}

void ScoreTextComponent::Initialize(const GameContext&)
{
	//Add Text Comp
	if (m_Score >= 0)
	{
		m_pTextComponent = new TextComponent(m_Font, L"+" + std::to_wstring(m_Score));		
	}
	else
	{
		m_pTextComponent = new TextComponent(m_Font, std::to_wstring(m_Score));
	}
	m_pTextComponent->SetColor(m_Color);
	GetGameObject()->AddComponent(m_pTextComponent);
}
void ScoreTextComponent::Update(const GameContext& gameContext)
{
	if (m_pTextComponent)
	{
		float elapsed = gameContext.pGameTime->GetElapsed();

		//Color
		m_Color = m_pTextComponent->GetColor();
		m_Color.w += elapsed * m_AlphaSpeed;
		m_pTextComponent->SetColor({ m_Color.x, m_Color.y, m_Color.z, m_Color.w });

		//Movement
		auto* pTransform = GetTransform();
		const auto& pos = pTransform->GetPosition();
		pTransform->Translate(pos.x + m_Velocity.x * elapsed, pos.y + m_Velocity.y * elapsed, pos.z + m_Velocity.z * elapsed);
		m_Velocity.x += m_Decay.x * elapsed;
		m_Velocity.y += m_Decay.y * elapsed;
		m_Velocity.z += m_Decay.z * elapsed;

		//Delete Self If invisible
		if (m_Color.w <= 0)
		{
			Destroyer::GetInstance().Destroy(GetGameObject());
		}
	}
}
void ScoreTextComponent::Draw(const GameContext&)
{
}
