#pragma once
class LerpComponent final : public BaseComponent
{
public:
	explicit LerpComponent(const DirectX::XMFLOAT3& targetPos, float speed, float allowed = FLT_EPSILON);
	virtual ~LerpComponent() = default;

	void SetTarget(const DirectX::XMFLOAT3& target) { m_TargetPosition = target; m_IsMoving = true; }
	const DirectX::XMFLOAT3& GetTarget() const { return m_TargetPosition; }
	void SetSpeed(float speed) { m_Speed = abs(speed); }
	float GetSpeed() const { return m_Speed; }
	void SetMoving(bool move) { m_IsMoving = move; }
	bool IsMoving() const { return m_IsMoving; }
	void SetOnDestination(const std::function<void()>& function) { m_OnDestination = function; }
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:
	float Lerp(float a, float b, float t) const;
	float SqDistance(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) const;

	DirectX::XMFLOAT3 m_TargetPosition;
	float m_Speed;
	float m_Allowed;
	bool m_IsMoving;
	std::function<void()> m_OnDestination = nullptr;
};

