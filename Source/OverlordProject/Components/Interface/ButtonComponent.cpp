#include "stdafx.h"
#include "ButtonComponent.h"
#include "TextComponent.h"
#include "InterfaceComponent.h"
#include "../../Singletons/AssetFiles.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "OverlordGame.h"
#include "SpriteFont.h"
#include "TextureData.h"

ButtonComponent::ButtonComponent(const std::wstring& text, const DirectX::XMFLOAT4& colorSelect,
	const DirectX::XMFLOAT4& colorUnselect, const std::function<void(ButtonComponent*)>& onClick)
	: m_Text(text)
	, m_pTextComponent(nullptr)
	, m_OnClick(onClick)
	, m_UnselectedColor(colorUnselect)
	, m_SelectedColor(colorSelect)
	, m_IsSelected(false)
{
}

void ButtonComponent::Click()
{
	m_IsSelected = false;
	if (m_pTextComponent)
	{
		m_pTextComponent->SetColor(m_UnselectedColor);
	}
	if (m_OnClick) m_OnClick(this);
}
void ButtonComponent::Select()
{
	m_IsSelected = true;
	if (m_pTextComponent) m_pTextComponent->SetColor(m_SelectedColor);
}
void ButtonComponent::Unselect()
{
	m_IsSelected = false;
	if (m_pTextComponent) m_pTextComponent->SetColor(m_UnselectedColor);
}

void ButtonComponent::Initialize(const GameContext&)
{
	GameObject* pObject = GetGameObject();

	//Button
	std::wstring asset{};
	float height = 0.0f;
	if (AssetFiles::GetInstance().Get(eAssetId::Texture_Menu_Button, asset))
	{
		InterfaceComponent* pVisuals = new InterfaceComponent(asset);
		pObject->AddComponent(pVisuals);
		height = pVisuals->GetTexture()->GetDimension().y;
	}

	//Text
	if (AssetFiles::GetInstance().Get(eAssetId::Font_Menu_ButtonText, asset))
	{
		GameObject* pTextObject = new GameObject();

		m_pTextComponent = new TextComponent(asset, m_Text);
		m_pTextComponent->SetCentered(true);
		pTextObject->AddComponent(m_pTextComponent);

		if (m_IsSelected)m_pTextComponent->SetColor(m_SelectedColor);
		else m_pTextComponent->SetColor(m_UnselectedColor);

		pTextObject->GetTransform()->Translate(0, -0.1f * height, 0);
		pObject->AddChild(pTextObject);
	}
}

