#pragma once
#include "../../Prototype/PrototypeComponent.h"
#include "HolderInfo.h"


class ParticleComponent;
class SliderParticleComponent;
class FadeParticleComponent;
class HolderProgressComponent final : public PrototypeComponent
{
	enum class eProgressState
	{
		Active, Ready, Overtime
	};
public:
	explicit HolderProgressComponent(const HolderInfo& info);
	virtual ~HolderProgressComponent() = default;
	HolderProgressComponent(const HolderProgressComponent& other) = delete;
	HolderProgressComponent(HolderProgressComponent&& other) noexcept = delete;
	HolderProgressComponent& operator=(const HolderProgressComponent& other) = delete;
	HolderProgressComponent& operator=(HolderProgressComponent&& other) noexcept = delete;
	virtual PrototypeComponent* Clone() const override;

	void UpdateTime(float totalAccu, float totalTarget);
	void UpdateState(float overtimeScale);
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:
	const HolderInfo& m_HolderInfo;
	eProgressState m_State;

	SliderParticleComponent* m_pTimerSlider = nullptr;
	ParticleComponent* m_pBackParticle = nullptr;
	FadeParticleComponent* m_pStateParticle = nullptr;


	bool ShowTimer(float fraction);
	void SetState(eProgressState state);
};

