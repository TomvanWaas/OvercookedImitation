#pragma once

class ParticleEmitterComponent;
class FireComponent final : public BaseComponent
{
public:
	explicit FireComponent(const std::wstring& assetFile, float spreadDelay, float spreadDistance, float extinguishDuration);
	virtual ~FireComponent() = default;

	void Extinguish(float elapsed);
	void Lit();
	bool IsLit() const { return m_IsLit; }
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void OnNotify(ObservedID observedID) override;
private:
	std::wstring m_AssetFile;
	float m_SpreadDelay;
	float m_SpreadAccu;
	float m_SpreadDistance;
	float m_ExtinguishDuration;
	float m_ExtinguishAccu;

	bool m_IsLit;
	bool m_HasSpread;
	ParticleEmitterComponent* m_pParticleEmitter;


	bool InDistance(FireComponent& pFire);


	void CheckIntensity() const;
};

