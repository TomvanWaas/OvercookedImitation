#pragma once
#include "ParticleComponent.h"
class FadeParticleComponent final : public ParticleComponent
{
public:
	enum class eFadeState
	{
		None, In, Out
	};
	explicit FadeParticleComponent(const std::wstring& assetFile, float speed, const std::function<void(eFadeState)>& onEnd = nullptr);
	virtual ~FadeParticleComponent() = default;
	FadeParticleComponent(const FadeParticleComponent& other) = delete;
	FadeParticleComponent(FadeParticleComponent&& other) noexcept = delete;
	FadeParticleComponent& operator=(const FadeParticleComponent& other) = delete;
	FadeParticleComponent& operator=(FadeParticleComponent&& other) noexcept = delete;

	void SetFadeInactive();
	bool IsFadeActive() const { return m_FadeState != eFadeState::None; }
	bool IsFadeIn() const { return m_FadeState == eFadeState::In; }
	bool IsFadeOut() const { return m_FadeState == eFadeState::Out; }
	void FadeIn();
	void FadeOut();
	void SetFadeSpeed(float speed) { m_FadeSpeed = speed; }
	float GetFadeSpeed() const { return m_FadeSpeed; }
	void SetOnFadeEnd(const std::function<void(eFadeState)>& func) { m_OnFadeEnd = func; }
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
private:
	float m_FadeSpeed;
	eFadeState m_FadeState;
	std::function<void(eFadeState)> m_OnFadeEnd;

};

