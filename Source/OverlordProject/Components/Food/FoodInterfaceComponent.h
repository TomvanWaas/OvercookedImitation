#pragma once
#include "FoodInfo.h"
#include "../../Prototype/PrototypeComponent.h"

class ColorMaterial;
class FoodComponent;
class HolderComponent;
enum class eConvertReturn;
class FoodInterfaceComponent : public PrototypeComponent
{
public:
	explicit FoodInterfaceComponent(const FoodInfo& info);
	virtual ~FoodInterfaceComponent() = default;
	FoodInterfaceComponent(const FoodInterfaceComponent& other) = delete;
	FoodInterfaceComponent(FoodInterfaceComponent&& other) noexcept = delete;
	FoodInterfaceComponent& operator=(const FoodInterfaceComponent& other) = delete;
	FoodInterfaceComponent& operator=(FoodInterfaceComponent&& other) noexcept = delete;
	virtual PrototypeComponent* Clone() const override;
	virtual void AsClone() override;

	//
	void Highlight(const DirectX::XMFLOAT4& color = { 1.25f, 1.25f, 1.25f, 1.0f });

	//
	eConvertReturn Convert(eFoodState state);
	eConvertReturn Convert(float elapsed, eUpdate update, HolderComponent* pHolder, FoodConversion& conversion);
	bool CanConvert(eUpdate update) const;

	//
	eFoodState GetState() const;
	eFoodType GetType() const;

	//GetComponent
	//************
	FoodComponent* GetFoodComponent() const { return m_pFoodComponent; }
	
protected:
	virtual void OnNotify(ObservedID observedID) override;
	virtual void Initialize(const GameContext&) override;
	virtual void Update(const GameContext&) override;
	virtual void Draw(const GameContext& gameContext) override;
	virtual void PostDraw(const GameContext&) override;
private:
	const FoodInfo& m_FoodInfo;
	bool m_IsHighlighted = false;

	ColorMaterial* m_pMaterial = nullptr;
	FoodComponent* m_pFoodComponent = nullptr;


	void HighlightEnable(const DirectX::XMFLOAT4& color);
	void HighlightDisable();
	void SwapModel(eFoodState state, const GameContext& gameContext);

};

