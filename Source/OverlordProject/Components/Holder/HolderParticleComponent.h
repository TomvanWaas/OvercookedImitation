#pragma once
#include "../../Prototype/PrototypeComponent.h"

enum class eAssetId;
class ParticleEmitterComponent;

class HolderParticleComponent final : public PrototypeComponent
{
	enum class eState
	{
		Up,
		Not,
		Down,
	};
public:
	explicit HolderParticleComponent(eAssetId particleID, const DirectX::XMFLOAT4& color = { 1,1,1,1 });
	virtual ~HolderParticleComponent() = default;
	virtual PrototypeComponent* Clone() const override;

	void Activate(float fadeTime);
	void Deactivate(float fadeTime);

protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:
	DirectX::XMFLOAT4 m_Color;
	float m_Speed;
	float m_Accu;
	eState m_State;
	eAssetId m_ParticleAsset;
	ParticleEmitterComponent* m_pParticleComponent;
};

