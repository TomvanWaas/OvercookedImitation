#pragma once

class TextComponent;
class ButtonComponent final : public BaseComponent
{
public:
	explicit ButtonComponent(const std::wstring& text, 
		const DirectX::XMFLOAT4& colorSelect,
		const DirectX::XMFLOAT4& colorUnselect, 
		const std::function<void(ButtonComponent*)>& onClick);
	virtual ~ButtonComponent() = default;

	void Click();
	void Select();
	void Unselect();

	void SetLeft(ButtonComponent* pButton) { m_pLeft = pButton; }
	void SetRight(ButtonComponent* pButton) { m_pRight = pButton; }
	void SetUp(ButtonComponent* pButton) { m_pUp = pButton; }
	void SetDown(ButtonComponent* pButton) { m_pDown = pButton; }
	ButtonComponent* GetLeft() const { return m_pLeft; }
	ButtonComponent* GetRight() const { return m_pRight; }
	ButtonComponent* GetUp() const { return m_pUp; }
	ButtonComponent* GetDown() const { return m_pDown; }
protected:
	virtual void Initialize(const GameContext& gameContext) override;
private:
	std::function<void(ButtonComponent*)> m_OnClick;
	std::wstring m_Text;
	DirectX::XMFLOAT4 m_SelectedColor;
	DirectX::XMFLOAT4 m_UnselectedColor;
	bool m_IsSelected;
	TextComponent* m_pTextComponent;

	//Surroundings
	ButtonComponent* m_pUp = nullptr;
	ButtonComponent* m_pDown = nullptr;
	ButtonComponent* m_pLeft = nullptr;
	ButtonComponent* m_pRight = nullptr;
};

