#pragma once
class CameraEventComponent final : public BaseComponent
{
	enum class eState
	{
		Go, Wait, Return, None
	};
public:
	void TurnLeft(float accel, float duration, float waitdelay);
	void TurnRight(float accel, float duration, float waitdelay);

	explicit CameraEventComponent() = default;
	virtual ~CameraEventComponent() = default;
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:
	float m_Accu = 0;
	float m_Acceleration = 0;
	float m_Speed = 0;
	float m_Duration = 0;
	float m_Delay = 0;
	eState m_State = eState::None;
};

