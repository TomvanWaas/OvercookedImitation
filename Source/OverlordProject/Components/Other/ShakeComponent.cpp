#include "stdafx.h"
#include "ShakeComponent.h"
#include "TransformComponent.h"



ShakeComponent::ShakeComponent(const DirectX::XMFLOAT3& force, float speed)
	: m_Force(force)
	, m_Accu(0)
	, m_Speed(abs(speed))
	, m_Time(-1)
	, m_Offset(0.0f, 0.0f, 0.0f)
{
}

void ShakeComponent::Enable(bool enable, float time)
{
	m_IsEnabled = enable;
	m_Time = time;
	if (enable == false)
	{
		const auto &pos = GetTransform()->GetPosition();
		GetTransform()->Translate(pos.x - m_Offset.x, pos.y - m_Offset.y, pos.z - m_Offset.z);
	}
	else
	{
		m_Offset = {};
	}
}

void ShakeComponent::Initialize(const GameContext&)
{
}

void ShakeComponent::Update(const GameContext& gameContext)
{
	if (m_IsEnabled)
	{
		m_Accu += gameContext.pGameTime->GetElapsed();

		auto* pTransform = GetTransform();
		DirectX::XMFLOAT3 pivot = pTransform->GetPosition();
		pivot.x -= m_Offset.x;
		pivot.y -= m_Offset.y;
		pivot.z -= m_Offset.z;

		float sin = sinf(m_Accu * m_Speed);
		m_Offset.x = sin * m_Force.x;
		m_Offset.y = sin * m_Force.y;
		m_Offset.z = sin * m_Force.z;
		pTransform->Translate(pivot.x + m_Offset.x, pivot.y + m_Offset.y, pivot.z + m_Offset.z);

		//End Timer
		if ((m_Accu >= m_Time))
		{
			Enable(false);
			m_Accu = 0;
			m_Time = -1;
		}
	}
}

void ShakeComponent::Draw(const GameContext&)
{
}
