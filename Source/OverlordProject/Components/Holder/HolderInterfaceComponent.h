#pragma once
#include "../../Prototype/PrototypeComponent.h"
#include "HolderInfo.h"
#include "ParticleEmitterComponent.h"
class FoodInterfaceComponent;
class BoxComponentBase;
class ColorMaterial;
class HolderComponent;
enum class eConvertReturn;
class HolderIconsComponent;
class HolderProgressComponent;
class HolderParticleComponent;

class HolderInterfaceComponent final : public PrototypeComponent
{
public:
	explicit HolderInterfaceComponent(const HolderInfo& info);
	virtual ~HolderInterfaceComponent() = default;
	HolderInterfaceComponent(const HolderInterfaceComponent& other) = delete;
	HolderInterfaceComponent(HolderInterfaceComponent&& other) noexcept = delete;
	HolderInterfaceComponent& operator=(const HolderInterfaceComponent& other) = delete;
	HolderInterfaceComponent& operator=(HolderInterfaceComponent&& other) noexcept = delete;
	virtual PrototypeComponent* Clone() const override;
	virtual void AsClone() override;

	void Highlight(const DirectX::XMFLOAT4& color = { 1.25f, 1.25f, 1.25f, 1.0f });

	bool AddFood(HolderInterfaceComponent& other);
	bool AddFood(FoodInterfaceComponent& food);
	FoodInterfaceComponent* RemoveFood();
	void Burn();

	eConvertReturn Convert(float elapsed, eUpdate update, BoxComponentBase* pBox);

	HolderComponent* GetHolderComponent() const { return m_pHolderComponent; }
	eHolderType GetType() const { return m_HolderInfo.type; }
protected:
	virtual void Initialize(const GameContext&) override;
	virtual void Update(const GameContext&) override;
	virtual void Draw(const GameContext& gameContext) override;
	virtual void PostDraw(const GameContext&) override;
private:
	const HolderInfo& m_HolderInfo;

	bool m_IsBeingConverted;
	bool m_IsHighlighted;
	bool m_IsBurned;
	
	ColorMaterial* m_pMaterial = nullptr;
	HolderComponent* m_pHolderComponent = nullptr;
	HolderIconsComponent* m_pIconsComponent = nullptr;
	HolderProgressComponent* m_pProgressComponent = nullptr;
	ParticleEmitterComponent* m_pParticleComponent = nullptr;

	void HighlightEnable(const DirectX::XMFLOAT4& color);
	void HighlightDisable();


	float OvertimeScale(float minOvertime) const;
};

