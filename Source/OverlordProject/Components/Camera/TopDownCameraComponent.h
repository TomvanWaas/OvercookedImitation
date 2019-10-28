#pragma once
template <typename T>
struct MinMax
{
	MinMax(const T& mi, const T& ma):min(mi), max(ma){}
	T min;
	T max;
};
class TopDownCameraComponent final : public BaseComponent
{
public:
	explicit TopDownCameraComponent(const MinMax<float>& offsetLimits, float angleSpeed, float offsetSpeed);
	virtual ~TopDownCameraComponent() = default;
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:
	//Basic Movement
	MinMax<float> m_OffsetLimits;
	DirectX::XMFLOAT3 m_Pivot;
	float m_AngleSpeed;
	float m_OffsetSpeed;
	float m_Accu;

	//Helpers
	float ModF(float f, float m, int precision = 100) const;
};

