#pragma once

class SpriteComponent;
class ColorFlickerComponent final : public BaseComponent
{
public:
	explicit ColorFlickerComponent(const DirectX::XMFLOAT4& colorA, const DirectX::XMFLOAT4& colorB, float speed = 1, std::vector<SpriteComponent*> pSprites = {});
	virtual ~ColorFlickerComponent() = default;

	void Enable(bool enable, float time = -1);
	bool IsEnabled() const { return m_IsEnabled; }

	void SetColorA(const DirectX::XMFLOAT4& color) { m_ColorA = color; }
	void SetColorB(const DirectX::XMFLOAT4& color) { m_ColorB = color; }
	void SetOnEnd(const std::function<void()>& function) { m_OnEnd = function; }
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:
	std::vector<SpriteComponent*> m_pSprites;
	DirectX::XMFLOAT4 m_ColorA;
	DirectX::XMFLOAT4 m_ColorB;
	float m_Speed;
	float m_Accu;
	float m_Time;
	bool m_IsEnabled = false;
	std::function<void()> m_OnEnd = nullptr;


	DirectX::XMFLOAT4 Lerp(DirectX::XMFLOAT4 a, DirectX::XMFLOAT4 b, float t) const;
};

