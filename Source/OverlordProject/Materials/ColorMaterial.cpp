#include "stdafx.h"
#include "ColorMaterial.h"
#include "ContentManager.h"
#include "TextureData.h"


ColorMaterial::ColorMaterial(const std::wstring& effectFile, const std::wstring& technique, 
	const DirectX::XMFLOAT4& color, const std::string& colorName, const std::string& textureName)
	: Material(effectFile, technique)
	, m_Color(color)
	, m_ColorName(colorName)
	, m_TextureName(textureName)
	, m_pTexture(nullptr)
	, m_pColorVariable(nullptr)
	, m_pTextureVariable(nullptr)
{}


void ColorMaterial::SetTexture(const std::wstring& path)
{
	m_pTexture = ContentManager::Load<TextureData>(path);
}

void ColorMaterial::LoadEffectVariables()
{
	m_pColorVariable = GetEffect()->GetVariableByName(m_ColorName.c_str())->AsVector();
	if (!m_pColorVariable) Logger::LogWarning(L"ColorMaterial::LoadEffectVariables >> Failed to load m_pColorVariable");

	m_pTextureVariable = GetEffect()->GetVariableByName(m_TextureName.c_str())->AsShaderResource();
	if (!m_pTextureVariable) Logger::LogWarning(L"ColorMaterial::LoadEffectVariables >> Failed to load m_pTextureVariable");
}

void ColorMaterial::UpdateEffectVariables(const GameContext&, ModelComponent*)
{
	if (m_pColorVariable)
	{
		m_pColorVariable->SetFloatVector((float*)&m_Color);
	}
	if (m_pTextureVariable && m_pTexture)
	{
		m_pTextureVariable->SetResource(m_pTexture->GetShaderResourceView());
	}
}
