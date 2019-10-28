#pragma once
class ItemInterfaceComponent;
class ItemPointer final
{
public:
	explicit ItemPointer() = default;
	virtual ~ItemPointer() = default;
	ItemPointer(const ItemPointer& other) = delete;
	ItemPointer(ItemPointer&& other) noexcept = delete;
	ItemPointer& operator=(const ItemPointer& other) = delete;
	ItemPointer& operator=(ItemPointer&& other) noexcept = delete;

	ItemInterfaceComponent* Get() const;
	GameObject* GetParent() const;
	ItemInterfaceComponent* Set(ItemInterfaceComponent* pItem);
	ItemInterfaceComponent* Remove();
	void Swap(ItemPointer& other);

	void Initialize(GameObject* pParent, const DirectX::XMFLOAT3& itemOffset);
	bool Initialized() const;
	bool IsValid() const;
private:
	DirectX::XMFLOAT3 m_Offset{};
	GameObject* m_pParent = nullptr;
	ItemInterfaceComponent* m_pItem = nullptr;

	void HelperRemove();
	void HelperAdd(ItemInterfaceComponent* pItem);
};

