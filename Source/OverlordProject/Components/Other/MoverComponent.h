#pragma once
class MoverComponent final : public BaseComponent
{
public:
	explicit MoverComponent() = default;
	virtual ~MoverComponent() = default;

	void Move(float accel, float reducedelay, const DirectX::XMFLOAT3& offset);
protected:
	virtual void Initialize(const GameContext&) override;
	virtual void Draw(const GameContext&) override {}
	virtual void Update(const GameContext& gameContext) override;
private:
	DirectX::XMFLOAT3 m_Pivot;
	DirectX::XMFLOAT3 m_Offset;
	float m_Accu;
	float m_Speed;
	float m_Acceleration;
	float m_ReduceDelay;


	bool m_Active = false;

	float SqDistance(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) const;
};

