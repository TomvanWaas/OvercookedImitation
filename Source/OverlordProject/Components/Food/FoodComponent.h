#pragma once
#include "FoodInfo.h"
#include "../../Prototype/PrototypeComponent.h"

enum class eConvertReturn
{
	Failed = 0,
	Converting = 1,
	Finished = 2
};

class HolderComponent;
class FoodComponent final : public PrototypeComponent
{
public:
	explicit FoodComponent(const FoodInfo& info, eFoodState state);
	virtual ~FoodComponent() = default;
	FoodComponent(const FoodComponent& other) = delete;
	FoodComponent(FoodComponent&& other) noexcept = delete;
	FoodComponent& operator=(const FoodComponent& other) = delete;
	FoodComponent& operator=(FoodComponent&& other) noexcept = delete;
	virtual PrototypeComponent* Clone() const override;

	void ReduceOvertime(float elapsed);
	eConvertReturn Convert(eFoodState state);
	eConvertReturn Convert(float elapsed, eUpdate update, HolderComponent* pHolder, FoodConversion& conversion);
	bool CanConvert(eUpdate update) const;

	float GetConversionAccu() const { return m_Accu; }
	float GetConversionTime() const { return m_Conversion.time; }

	eFoodState GetState() const { return m_State; }
protected:
	virtual void Initialize(const GameContext&) override;
	virtual void Update(const GameContext&) override;
	virtual void Draw(const GameContext&) override;
private:
	const FoodInfo& m_FoodInfo;
	FoodConversion m_Conversion;
	float m_Accu;
	eFoodState m_State;

	bool FindConversion(eUpdate update, FoodConversion& holder) const;
	void SetState(eFoodState state);
};

