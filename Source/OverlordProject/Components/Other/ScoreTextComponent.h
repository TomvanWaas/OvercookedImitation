#pragma once
class TextComponent;
class ScoreTextComponent final : public BaseComponent
{
public:
	explicit ScoreTextComponent(int score,
		const std::wstring& font, 
		const DirectX::XMFLOAT4& color, 
		const DirectX::XMFLOAT3& veloc, 
		const DirectX::XMFLOAT3& decay,
		float alphaSpeed = -0.25f);
	virtual ~ScoreTextComponent() = default;
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:
	std::wstring m_Font;
	DirectX::XMFLOAT4 m_Color;
	DirectX::XMFLOAT3 m_Velocity;
	DirectX::XMFLOAT3 m_Decay;
	float m_AlphaSpeed;
	int m_Score;
	TextComponent* m_pTextComponent;
};

