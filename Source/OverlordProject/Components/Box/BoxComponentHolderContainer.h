#pragma once
#include "../Box/BoxComponentBase.h"

class BoxComponentHolderContainer final : public BoxComponentBase
{
public:
	explicit BoxComponentHolderContainer(eBoxType type,
		const DirectX::XMFLOAT3& holderOffset,
		const std::vector<eHolderType>& allowedHolders,
		const ModelInfo& initInfo,
		eHolderType templateType,
		int templateCount = -1);
	virtual ~BoxComponentHolderContainer() = default;
	BoxComponentHolderContainer(const BoxComponentHolderContainer& other) = delete;
	BoxComponentHolderContainer(BoxComponentHolderContainer&& other) noexcept = delete;
	BoxComponentHolderContainer& operator=(const BoxComponentHolderContainer& other) = delete;
	BoxComponentHolderContainer& operator=(BoxComponentHolderContainer&& other) noexcept = delete;

	void AddTemplateCount(int count);
	eHolderType GetTemplateType() const { return m_TemplateType; }

	virtual bool AddFood(HolderPointer& other) override;
	virtual bool TakeFood(HolderPointer& other) override;
	virtual bool AddItem(ItemPointer&) override { return false; }
	virtual bool TakeItem(ItemPointer&) override { return false; }
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
private:
	eHolderType m_TemplateType;
	int m_TemplateCount;
	HolderInterfaceComponent* m_pCurrentClone = nullptr;



	void AddTemplateClone();
};

