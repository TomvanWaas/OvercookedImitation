#include "stdafx.h"
#include "LerpComponent.h"
#include "TransformComponent.h"

LerpComponent::LerpComponent(const DirectX::XMFLOAT3& targetPos, float speed, float allowed)
	: m_TargetPosition(targetPos)
	, m_Speed(speed)
	, m_Allowed(allowed)
	, m_IsMoving(true)
{
}

void LerpComponent::Initialize(const GameContext&)
{
}

void LerpComponent::Update(const GameContext& gameContext)
{
	if (m_IsMoving)
	{
		const float elapsed = gameContext.pGameTime->GetElapsed();
		auto* pTransform = GetTransform();
		const auto& currentPosition = pTransform->GetPosition();

		float sqDistance = SqDistance(currentPosition, m_TargetPosition);
		if (sqDistance > m_Allowed)
		{
			DirectX::XMFLOAT3 newPosition{};
			float lerp = elapsed * m_Speed;
			newPosition.x = Lerp(currentPosition.x, m_TargetPosition.x, lerp);
			newPosition.y = Lerp(currentPosition.y, m_TargetPosition.y, lerp);
			newPosition.z = Lerp(currentPosition.z, m_TargetPosition.z, lerp);
			pTransform->Translate(newPosition);
		}
		else
		{
			if (m_OnDestination) m_OnDestination();
			m_IsMoving = false;
		}
	}
}

void LerpComponent::Draw(const GameContext&)
{
}

float LerpComponent::Lerp(float a, float b, float t) const
{
	return ((1 - t)*a + t * b);
}

float LerpComponent::SqDistance(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) const
{
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);
}
