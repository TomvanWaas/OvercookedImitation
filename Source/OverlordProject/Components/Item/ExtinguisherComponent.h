#pragma once
#include "ItemInterfaceComponent.h"
#include "ParticleEmitterComponent.h"
class FireComponent;
class ColorMaterial;
class ExtinguisherComponent final : public ItemInterfaceComponent
{
public:
	explicit ExtinguisherComponent(float range = 15, float angle = 70, float speed = 1);
	virtual ~ExtinguisherComponent() = default;

	virtual void Highlight(const DirectX::XMFLOAT4& color) override;
	virtual bool Use(const GameContext& gameContext) override;
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void PostDraw(const GameContext& gameContext) override;
private:
	std::vector<FireComponent*> m_pFires;
	float m_Range;
	float m_Angle;
	float m_Speed;
	bool m_IsHighlighted = false;

	ColorMaterial* m_pMaterial = nullptr;
	ParticleEmitterComponent* m_pParticleEmitter = nullptr;

	bool InArea(FireComponent& fire) const;
};

