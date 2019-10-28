#include "stdafx.h"
#include "CameraEventComponent.h"
#include "TransformComponent.h"

void CameraEventComponent::TurnLeft(float accel, float duration, float waitdelay)
{
	m_Accu = 0;
	m_Speed = 0.0f;
	m_Acceleration = -0.035f;
	m_State = eState::Go;
	m_Acceleration = -abs(accel);
	m_Duration = duration;
	m_Delay = waitdelay;
}

void CameraEventComponent::TurnRight(float accel, float duration, float waitdelay)
{
	m_Accu = 0;
	m_Speed = 0.0f;
	m_State = eState::Go;
	m_Acceleration = abs(accel);
	m_Duration = duration;
	m_Delay = waitdelay;
}

void CameraEventComponent::Initialize(const GameContext&)
{
}

void CameraEventComponent::Update(const GameContext& gameContext)
{
	if (m_State == eState::None)return;

	const float elapsed = gameContext.pGameTime->GetElapsed();

	switch (m_State)
	{
	case eState::Go:
		{
			m_Accu += elapsed;
			if (m_Accu >= m_Duration)
			{
				m_Accu = 0.0f;
				m_State = eState::Wait;
				m_Speed = 0.0f;
				m_Acceleration = -m_Acceleration;
				return;
			}
		}
		break;
	case eState::Wait:
		{
			m_Accu += elapsed;
			if (m_Accu >= m_Delay)
			{
				m_Accu = 0.0f;
				m_State = eState::Return;
			}
			return;
		}
	case eState::Return:
		{
			m_Accu += elapsed;
			if (m_Accu >= m_Duration)
			{
				m_Accu = 0.0f;
				m_State = eState::None;
				return;
			}
		}
	}




	//Base Update
	m_Speed += m_Acceleration * elapsed;
	DirectX::XMVECTOR forwardV3 = DirectX::XMLoadFloat3(&GetTransform()->GetForward());
	DirectX::XMVECTOR rotationV4 = DirectX::XMQuaternionRotationAxis(forwardV3,DirectX::XMConvertToRadians(m_Speed));
	DirectX::XMVECTOR currentV4 = DirectX::XMLoadFloat4(&GetTransform()->GetRotation());
	GetTransform()->Rotate(DirectX::XMQuaternionMultiply(currentV4, rotationV4), true);
	if (m_Accu <= 0.0f)
	{
		//Instantiate Return
	}
}

void CameraEventComponent::Draw(const GameContext&)
{
}

