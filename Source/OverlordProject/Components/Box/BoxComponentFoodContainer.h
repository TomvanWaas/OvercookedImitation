#pragma once
#include "BoxComponentBase.h"

class TextureMaterial;
class BoxComponentFoodContainer final : public BoxComponentBase
{
public:
	explicit BoxComponentFoodContainer(eBoxType type,
		const DirectX::XMFLOAT3& holderOffset,
		const std::vector<eHolderType>& allowedHolders,
		const ModelInfo& initInfo,
		eFoodType templateType,
		const std::function<bool(const HolderPointer& holder)>& requirement);
	virtual ~BoxComponentFoodContainer() = default;
	BoxComponentFoodContainer(const BoxComponentFoodContainer& other) = delete;
	BoxComponentFoodContainer(BoxComponentFoodContainer&& other) noexcept = delete;
	BoxComponentFoodContainer& operator=(const BoxComponentFoodContainer& other) = delete;
	BoxComponentFoodContainer& operator=(BoxComponentFoodContainer&& other) noexcept = delete;

	virtual bool TakeFood(HolderPointer& other) override;
protected:
	virtual void Initialize(const GameContext& gameContext) override;

	virtual void EnableHighlight(const DirectX::XMFLOAT4& color) override;
	virtual void DisableHighlight() override;
private:
	eFoodType m_TemplateType;
	TextureMaterial* m_pTemplateMaterial;
	std::function<bool(const HolderPointer& holder)> m_TemplateRequirement;
};

