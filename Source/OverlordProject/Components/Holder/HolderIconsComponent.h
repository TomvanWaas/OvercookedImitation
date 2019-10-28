#pragma once
#include "HolderInfo.h"
#include "../../Prototype/PrototypeComponent.h"

class HolderComponent;
class ParticleComponent;
enum class eFoodType;
enum class eFoodState;
class HolderIconsComponent final : public PrototypeComponent
{
public:
	explicit HolderIconsComponent(const HolderInfo& info, int size, const std::function<bool(eFoodType, eFoodState)>& showRequirement);
	virtual ~HolderIconsComponent() = default;
	HolderIconsComponent(const HolderIconsComponent& other) = delete;
	HolderIconsComponent(HolderIconsComponent&& other) noexcept = delete;
	HolderIconsComponent& operator=(const HolderIconsComponent& other) = delete;
	HolderIconsComponent& operator=(HolderIconsComponent&& other) noexcept = delete;
	virtual PrototypeComponent* Clone() const override;
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
	virtual void OnNotify(ObservedID observedID) override;
private:
	const HolderInfo& m_HolderInfo;
	std::vector<ParticleComponent*> m_pParticles;
	std::function<bool(eFoodType, eFoodState)> m_ShowRequirement;
	HolderComponent* m_pHolderComponent = nullptr;

	DirectX::XMFLOAT2 IconOffset(int iconIndex, int maxIndex, float offset) const;
};

