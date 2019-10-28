#pragma once
#include "../Food/FoodPointer.h"
#include "../../Structs//Recipe.h"
#include "HolderInfo.h"
#include "../../Prototype/PrototypeComponent.h"

class BoxComponentBase;
enum class eConvertReturn;
class HolderComponent final : public PrototypeComponent
{
public:
	explicit HolderComponent(const HolderInfo& info);
	virtual ~HolderComponent();
	HolderComponent(const HolderComponent& other) = delete;
	HolderComponent(HolderComponent&& other) noexcept = delete;
	HolderComponent& operator=(const HolderComponent& other) = delete;
	HolderComponent& operator=(HolderComponent&& other) noexcept = delete;
	virtual PrototypeComponent* Clone() const override;

	void Highlight(const DirectX::XMFLOAT4& color);

	bool AddFood(HolderComponent& other);
	bool AddFood(FoodInterfaceComponent* pFood);
	FoodInterfaceComponent* RemoveFood();
	void BurnFood();

	eConvertReturn Convert(float elapsed, eUpdate update, BoxComponentBase* pBox);

	
	int GetIndex() const;
	int GetMaxIndex() const;
	Recipe GetRecipe() const;
	const FoodPointer& GetFoodPointer(int idx) const;

protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:

	const HolderInfo& m_HolderInfo;
	FoodPointer* m_pFoodPointers;
	int m_Index = 0;

	bool CanContain(eFoodType type, eFoodState state, eFoodState& result) const;
	bool ContainsBurned() const;

};

