#pragma once
class HolderInterfaceComponent;
class HolderPointer final
{
public:
	explicit HolderPointer() = default;
	virtual ~HolderPointer() = default;
	HolderPointer(const HolderPointer& other) = delete;
	HolderPointer(HolderPointer&& other) noexcept = delete;
	HolderPointer& operator=(const HolderPointer& other) = delete;
	HolderPointer& operator=(HolderPointer&& other) noexcept = delete;

	HolderInterfaceComponent* Get() const;
	GameObject* GetParent() const;
	HolderInterfaceComponent* Set(HolderInterfaceComponent* pHolder);
	HolderInterfaceComponent* Remove();
	void Swap(HolderPointer& other);

	void Initialize(GameObject* pParent, const DirectX::XMFLOAT3& holderOffset);
	bool Initialized() const;
	bool IsValid() const;
	bool IsEmpty() const;
private:
	DirectX::XMFLOAT3 m_Offset{};
	GameObject* m_pParent = nullptr;
	HolderInterfaceComponent* m_pHolder = nullptr;

	void HelperRemove();
	void HelperAdd(HolderInterfaceComponent* pHolder);
};

