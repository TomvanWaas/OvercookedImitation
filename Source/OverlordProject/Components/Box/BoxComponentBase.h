#pragma once
#include "../../Helpers/Enums.h"
#include "../../Structs//ModelInfo.h"
#include "../Holder/HolderPointer.h"
#include "../Item/ItemPointer.h"
class HolderInterfaceComponent;
class ColorMaterial;
class PrototypeObject;


class BoxComponentBase : public BaseComponent
{
public:
	explicit BoxComponentBase(eBoxType type, 
		const DirectX::XMFLOAT3& objectOffset, 
		const std::vector<eHolderType>& allowedHolders,
		const ModelInfo& initInfo);
	virtual ~BoxComponentBase() = default;
	BoxComponentBase(const BoxComponentBase& other) = delete;
	BoxComponentBase(BoxComponentBase&& other) noexcept = delete;
	BoxComponentBase& operator=(const BoxComponentBase& other) = delete;
	BoxComponentBase& operator=(BoxComponentBase&& other) noexcept = delete;
	

	eBoxType GetType()const { return m_Type; }

	//Interface
	void Highlight(const DirectX::XMFLOAT4& color = { 1.25f, 1.25f, 1.25f, 1.0f });
	virtual bool Interact(float elapsed);
	virtual bool AddFood(HolderPointer& other);
	virtual bool TakeFood(HolderPointer& other);
	virtual bool AddItem(ItemPointer& other);
	virtual bool TakeItem(ItemPointer& other);

	bool InitializeItem(ItemInterfaceComponent* pItem);
	bool InitializeHolder(HolderInterfaceComponent* pHolder);
protected:
	virtual void OnNotify(ObservedID observedID) override;
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
	void PostDraw(const GameContext&) override;

	HolderPointer m_HolderPointer;
	ItemPointer m_ItemPointer;
	DirectX::XMFLOAT3 m_ObjectOffset;
	std::vector<eHolderType> m_AllowedHolders;

	virtual void EnableHighlight(const DirectX::XMFLOAT4& color);
	virtual void DisableHighlight();
	bool IsOnFire() const { return m_CaughtFire; }

	void AddSound();
	void TakeSound();
private:
	eBoxType m_Type;
	ColorMaterial* m_pMaterial = nullptr;
	ModelInfo m_ModelInfo;
	bool m_IsHighlighted = false;
	bool m_CaughtFire = false;
	
	//Helpers
	void InitializeHolder(eHolderType type);
	void InitializeModel(const GameContext& gameContext);

};

