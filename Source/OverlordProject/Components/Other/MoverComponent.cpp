#include "stdafx.h"
#include "MoverComponent.h"
#include "TransformComponent.h"


void MoverComponent::Move(float accel, float reducedelay, const DirectX::XMFLOAT3& offset)
{
	m_Acceleration = accel;
	m_ReduceDelay = reducedelay;
	m_Offset = offset;
	m_Pivot = GetTransform()->GetPosition();
	m_Accu = 0.0f;
	m_Speed = 0.0f;
	m_Active = true;
}

void MoverComponent::Initialize(const GameContext&)
{
}

void MoverComponent::Update(const GameContext& gameContext)
{
	if (!m_Active) return;

	TransformComponent* pTransform = GetTransform();
	const float elapsed = gameContext.pGameTime->GetElapsed();

	DirectX::XMFLOAT3 target = m_Pivot;
	target.x += m_Offset.x;
	target.y += m_Offset.y;
	target.z += m_Offset.z;

	bool reducing = m_Accu >= m_ReduceDelay;
	m_Accu += elapsed * m_Speed;
	m_Speed += m_Acceleration * elapsed;

	if (!reducing && m_Accu >= m_ReduceDelay)
	{
		m_Acceleration = -m_Acceleration;
	}

	if (m_Accu >= 1.0f) 
	{
		m_Accu = 1.0f;
		m_Active = false;
	}
	DirectX::XMFLOAT3 pos{};
	pos.x = (1 - m_Accu) * m_Pivot.x + m_Accu * target.x;
	pos.y = (1 - m_Accu) * m_Pivot.y + m_Accu * target.y;
	pos.z = (1 - m_Accu) * m_Pivot.z + m_Accu * target.z;
	pTransform->Translate(pos);
}

float MoverComponent::SqDistance(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) const
{
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);
}
