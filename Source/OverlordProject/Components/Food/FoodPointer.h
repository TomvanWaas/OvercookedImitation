#pragma once
class FoodInterfaceComponent;
class FoodPointer final
{
public:
	explicit FoodPointer() = default;
	virtual ~FoodPointer() = default;
	FoodPointer(const FoodPointer& other) = delete;
	FoodPointer(FoodPointer&& other) noexcept = delete;
	FoodPointer& operator=(const FoodPointer& other) = delete;
	FoodPointer& operator=(FoodPointer&& other) noexcept = delete;

	FoodInterfaceComponent* Get() const;
	GameObject* GetParent() const;
	FoodInterfaceComponent* Set(FoodInterfaceComponent* pFood);
	FoodInterfaceComponent* Remove();
	void Swap(FoodPointer& other);

	void Initialize(GameObject* pParent, const DirectX::XMFLOAT3& foodOffset);
	bool Initialized() const;
	bool IsValid() const;
private:
	DirectX::XMFLOAT3 m_Offset{};
	GameObject* m_pParent = nullptr;
	FoodInterfaceComponent* m_pFoodObject = nullptr;


	void HelperRemove();
	void HelperAdd(FoodInterfaceComponent* pFood);
};

