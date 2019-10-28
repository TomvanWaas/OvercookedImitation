#pragma once

class ButtonComponent;
class ButtonManagerComponent final : public BaseComponent
{
	struct FromTo final
	{
		int from;
		int to;
	};
public:
	explicit ButtonManagerComponent(ButtonComponent* pCurrent, int inputID = 0);
	virtual ~ButtonManagerComponent() = default;

protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
private:
	FromTo m_Up;
	FromTo m_Down;
	FromTo m_Left;
	FromTo m_Right;
	FromTo m_Click;

	int m_InputID;
	ButtonComponent* m_pCurrent;
	ButtonComponent* m_pStart;

	bool HandleDirection(const FromTo& dir, ButtonComponent* pNext, InputManager* pInput);
};

