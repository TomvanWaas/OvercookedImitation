#include "stdafx.h"
#include "ButtonManagerComponent.h"
#include "ButtonComponent.h"

ButtonManagerComponent::ButtonManagerComponent(ButtonComponent* pCurrent, int inputID)
	: m_InputID(inputID)
	, m_pCurrent(pCurrent)
	, m_pStart(pCurrent)
	, m_Up()
	, m_Down()
	, m_Left()
	, m_Right()
	, m_Click()
{
	if (pCurrent) pCurrent->Select();
}

void ButtonManagerComponent::Initialize(const GameContext& gameContext)
{
	//Initialize Input
	InputManager* pInput = gameContext.pInput;
	const int nrPlayers = 4;
	int id = m_InputID;

	//Click button
	m_Click.from = id;
	for (int i = 0; i < nrPlayers; ++i)
	{
		pInput->AddInputAction(InputAction{ id, InputTriggerState::Pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A, GamepadIndex(i) });
		++id;
	}
	m_Click.to = id;

	//Down Button
	m_Down.from = id;
	for (int i = 0; i < nrPlayers; ++i)
	{
		pInput->AddInputAction(InputAction{ id, InputTriggerState::Pressed, 'S', -1, XINPUT_GAMEPAD_DPAD_DOWN });
		++id;
	}
	m_Down.to = id;

	//Up Button
	m_Up.from = id;
	for (int i = 0; i < nrPlayers; ++i)
	{
		pInput->AddInputAction(InputAction{ id, InputTriggerState::Pressed, 'W' , -1, XINPUT_GAMEPAD_DPAD_UP });
		++id;
	}
	m_Up.to = id;

	//Left Button
	m_Left.from = id;
	for (int i = 0; i < nrPlayers; ++i)
	{
		pInput->AddInputAction(InputAction{ id, InputTriggerState::Pressed, 'A' , -1, XINPUT_GAMEPAD_DPAD_LEFT });
		++id;
	}
	m_Left.to = id;

	//Right Button
	m_Right.from = id;
	for (int i = 0; i < nrPlayers; ++i)
	{
		pInput->AddInputAction(InputAction{ id, InputTriggerState::Pressed, 'D' , -1, XINPUT_GAMEPAD_DPAD_RIGHT });
		++id;
	}
	m_Right.to = id;
}
void ButtonManagerComponent::Update(const GameContext& gameContext)
{
	if (!m_pCurrent) return;

	//Read Input
	InputManager* pInput = gameContext.pInput;

	//Self
	//****
	//- Click
	for (int i = m_Click.from; i < m_Click.to; ++i)
	{
		if (pInput->IsActionTriggered(i))
		{
			m_pCurrent->Click();
			m_pCurrent->Unselect();
			if (m_pStart)
			{
				m_pCurrent = m_pStart;
			}
			m_pCurrent->Select();
			return;
		}
	}

	//Other
	//*****
	//- Down
	//- Up
	//- Right
	//- Left
	if (HandleDirection(m_Down, m_pCurrent->GetDown(), pInput))return;
	if (HandleDirection(m_Up, m_pCurrent->GetUp(), pInput))return;
	if (HandleDirection(m_Right, m_pCurrent->GetRight(), pInput))return;
	if (HandleDirection(m_Left, m_pCurrent->GetLeft(), pInput))return;

}

bool ButtonManagerComponent::HandleDirection(const FromTo& dir, ButtonComponent* pNext, InputManager* pInput)
{
	if (!pNext || !pInput) return false;
	for (int i = dir.from; i < dir.to; ++i)
	{
		if (pInput->IsActionTriggered(i))
		{
			m_pCurrent->Unselect();
			m_pCurrent = pNext;
			m_pCurrent->Select();
		}
	}
	return false;
}


