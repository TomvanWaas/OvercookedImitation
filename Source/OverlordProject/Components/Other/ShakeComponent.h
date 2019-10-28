#pragma once
class ShakeComponent final : public BaseComponent
{
public:
	explicit ShakeComponent(const DirectX::XMFLOAT3& force, float speed);
	virtual ~ShakeComponent() = default;

	void Enable(bool enable, float time = -1);
	bool IsEnabled() const { return m_IsEnabled; }
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:
	DirectX::XMFLOAT3 m_Force;
	DirectX::XMFLOAT3 m_Offset;
	float m_Accu;
	float m_Speed;
	float m_Time;
	bool m_IsEnabled = false;
};

