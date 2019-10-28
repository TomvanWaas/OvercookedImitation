#pragma once
#include "BoxComponentBase.h"

enum class eSoundID;
class BoxComponentActiveUpdator final : public BoxComponentBase
{
public:
	explicit BoxComponentActiveUpdator(eBoxType type,
		const DirectX::XMFLOAT3& holderOffset,
		const std::vector<eHolderType>& allowedHolders,
		const ModelInfo& initInfo,
		eUpdate updateType, 
		float updateSpeed,
		float updateDelay,
		const std::function<void(const HolderPointer&)>& onUpdate = nullptr,
		const std::function<void(const HolderPointer&)>& onFinished = nullptr);
	virtual ~BoxComponentActiveUpdator() = default;
	BoxComponentActiveUpdator(const BoxComponentActiveUpdator& other) = delete;
	BoxComponentActiveUpdator(BoxComponentActiveUpdator&& other) noexcept = delete;
	BoxComponentActiveUpdator& operator=(const BoxComponentActiveUpdator& other) = delete;
	BoxComponentActiveUpdator& operator=(BoxComponentActiveUpdator&& other) noexcept = delete;

	eUpdate GetUpdateType() const { return m_UpdateType; }
	float GetUpdateSpeed() const { return m_UpdateSpeed; }
	float GetUpdateDelay() const { return m_UpdateDelay; }
	void SetUpdateType(eUpdate type) { m_UpdateType = type; }
	void SetUpdateSpeed(float speed) { m_UpdateSpeed = speed; }
	void SetUpdateDelay(float delay) { m_UpdateDelay = delay; }

	virtual bool AddFood(HolderPointer& other) override;
	virtual bool TakeFood(HolderPointer& other) override;
	virtual bool Interact(float elapsed) override;
	virtual bool AddItem(ItemPointer&) override { return false; }
	virtual bool TakeItem(ItemPointer&) override { return false; }
private:
	float m_UpdateSpeed;
	float m_UpdateDelay;
	float m_Accu = 0.0f;
	eUpdate m_UpdateType;
	bool m_Finished ;
	std::function<void(const HolderPointer&)> m_OnUpdate;
	std::function<void(const HolderPointer&)> m_OnFinished;
	eSoundID m_InteractSound;

	void InteractSound();
};

