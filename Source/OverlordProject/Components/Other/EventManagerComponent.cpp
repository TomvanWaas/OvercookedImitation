#include "stdafx.h"
#include "EventManagerComponent.h"
#include "GameObject.h"
#include "GameScene.h"
#include "MoverComponent.h"
#include "../Camera/CameraEventComponent.h"


EventManagerComponent::EventManagerComponent(float delay, bool startLeft)
	: m_Delay(delay)
	, m_MoveLeft(startLeft)
	, m_Accu(0.0f)
	, m_pCamereEvent(nullptr)
	, m_pMoveables()
{
}

void EventManagerComponent::Initialize(const GameContext&)
{
	auto* pScene = GetGameObject()->GetScene();
	m_pCamereEvent = pScene->GetDynamicObjectWith<CameraEventComponent>();
	m_pMoveables = pScene->GetDynamicObjectsWith<MoverComponent>(true);
}
void EventManagerComponent::Update(const GameContext& gameContext)
{
	const float elapsed = gameContext.pGameTime->GetElapsed();
	m_Accu += elapsed;
	if (m_Accu >= m_Delay)
	{
		m_Accu -= m_Delay;
		m_MoveLeft = !m_MoveLeft;

		//Values
		const float cameraAcceleration{0.01f};
		const float cameraDuration{2.5f};
		const float cameraWait{0.75f};
		const float moveAcceleration{0.33f};
		const float moveDuration{0.80f};
		const DirectX::XMFLOAT3 moveOffset{ 30,0,0 };

		//Left
		if (m_MoveLeft)
		{
			if (m_pCamereEvent)
			{
				m_pCamereEvent->TurnRight(cameraAcceleration, cameraDuration, cameraWait);
			}
			for (size_t i = 0, j = m_pMoveables.size(); i < j; ++i)
			{
				MoverComponent* pMoveable = m_pMoveables[i];
				pMoveable->Move(moveAcceleration, moveDuration, moveOffset);
			}
		}
		//Right
		else
		{
			if (m_pCamereEvent)
			{
				m_pCamereEvent->TurnLeft(cameraAcceleration, cameraDuration, cameraWait);
			}
			for (size_t i = 0, j = m_pMoveables.size(); i < j; ++i)
			{
				MoverComponent* pMoveable = m_pMoveables[i];
				DirectX::XMFLOAT3 offset{ -moveOffset.x, -moveOffset.y, -moveOffset.z };
				pMoveable->Move(moveAcceleration, moveDuration, offset);
			}
		}
	}


}
void EventManagerComponent::Draw(const GameContext&)
{
}



