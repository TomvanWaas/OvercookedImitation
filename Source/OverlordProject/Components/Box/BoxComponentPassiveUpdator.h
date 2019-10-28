#pragma once
#include "BoxComponentBase.h"

class BoxComponentPassiveUpdator final : public BoxComponentBase
{
public:
	explicit BoxComponentPassiveUpdator(eBoxType type,
		const DirectX::XMFLOAT3& holderOffset,
		const std::vector<eHolderType>& allowedHolders,
		const ModelInfo& initInfo,
		eUpdate updateType, float updateSpeed);
	virtual ~BoxComponentPassiveUpdator() = default;
	BoxComponentPassiveUpdator(const BoxComponentPassiveUpdator& other) = delete;
	BoxComponentPassiveUpdator(BoxComponentPassiveUpdator&& other) noexcept = delete;
	BoxComponentPassiveUpdator& operator=(const BoxComponentPassiveUpdator& other) = delete;
	BoxComponentPassiveUpdator& operator=(BoxComponentPassiveUpdator&& other) noexcept = delete;

	virtual bool AddFood(HolderPointer& other) override;
	virtual bool AddItem(ItemPointer&) override { return false; }
	virtual bool TakeItem(ItemPointer&) override { return false; }

	eUpdate GetUpdateType() const { return m_UpdateType; }
	float GetUpdateSpeed() const { return m_UpdateSpeed; }
	void SetUpdateType(eUpdate type) { m_UpdateType = type; }
	void SetUpdateSpeed(float speed) { m_UpdateSpeed = speed; }
protected:
	virtual void Update(const GameContext& gameContext) override;
private:
	float m_UpdateSpeed;
	eUpdate m_UpdateType;


};

