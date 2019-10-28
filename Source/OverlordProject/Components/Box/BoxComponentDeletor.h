#pragma once
#include "../Box/BoxComponentBase.h"

class BoxComponentDeletor final : public BoxComponentBase
{
public:
	enum class eDeleteSpecifier
	{
		Nothing, Food, All
	};
	explicit BoxComponentDeletor(eBoxType type,
		const DirectX::XMFLOAT3& holderOffset,
		const std::vector<eHolderType>& allowedHolders,
		const ModelInfo& initInfo,
		const std::map<eHolderType, eDeleteSpecifier>& deleteInfo,
		const DirectX::XMFLOAT3& startOffset,
		const DirectX::XMFLOAT3& endOffset,
		std::function<void(const HolderPointer&)> onRemove);
	virtual ~BoxComponentDeletor() = default;
	BoxComponentDeletor(const BoxComponentDeletor& other) = delete;
	BoxComponentDeletor(BoxComponentDeletor&& other) noexcept = delete;
	BoxComponentDeletor& operator=(const BoxComponentDeletor& other) = delete;
	BoxComponentDeletor& operator=(BoxComponentDeletor&& other) noexcept = delete;

	virtual bool AddFood(HolderPointer& other) override;
	virtual bool TakeFood(HolderPointer& other) override;
	virtual bool AddItem(ItemPointer&) override { return false; }
	virtual bool TakeItem(ItemPointer&) override { return false; }
protected:
	virtual void Update(const GameContext& gameContext) override;
private:
	std::map<eHolderType, eDeleteSpecifier> m_DeleteInfo;
	std::vector<std::pair<GameObject*, float>> m_pObjectsToDelete;
	DirectX::XMFLOAT3 m_StartOffset;
	DirectX::XMFLOAT3 m_EndOffset;
	std::function<void(const HolderPointer&)> m_OnRemove;


	void AddToDelete(GameObject* pObject);
	DirectX::XMFLOAT3 Lerp(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b, float t) const;
	bool IsOnEnd(const DirectX::XMFLOAT3& p, float offset) const;
};

