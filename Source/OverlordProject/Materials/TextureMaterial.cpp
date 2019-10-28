#include "stdafx.h"
#include "TextureMaterial.h"
#include "TextureData.h"
#include "ContentManager.h"

ID3DX11EffectVectorVariable* TextureMaterial::m_pColorVariable = nullptr;
ID3DX11EffectVectorVariable* TextureMaterial::m_pPivotVariable = nullptr;
ID3DX11EffectVectorVariable* TextureMaterial::m_pSizeVariable = nullptr;
ID3DX11EffectVectorVariable* TextureMaterial::m_pTextureSizeVariable = nullptr;
ID3DX11EffectShaderResourceVariable* TextureMaterial::m_pTextureVariable = nullptr;

TextureMaterial::TextureMaterial(const std::wstring& texture, const DirectX::XMFLOAT2& pivot, const DirectX::XMFLOAT2& size,
	const DirectX::XMFLOAT4& color, bool doubleSided)
	: Material(L"./Resources/Effects/TextureEffect.fx", (doubleSided)? (L"DoubleSided") : (L"OneSided"))
	, m_pTexture(ContentManager::Load<TextureData>(texture))
	, m_Pivot(pivot)
	, m_Size(size)
	, m_Color(color)
{
}

TextureMaterial::TextureMaterial(const std::wstring& texture, bool doubleSided)
	: Material(L"./Resources/Effects/TextureEffect.fx", (doubleSided) ? (L"DoubleSided") : (L"OneSided"))
	, m_pTexture(ContentManager::Load<TextureData>(texture))
	, m_Pivot({ 0,0 })
	, m_Size({ 1,1 })
	, m_Color({ 1,1,1,1 })
{
}

void TextureMaterial::SetTexture(const std::wstring& texture)
{
	m_pTexture = ContentManager::Load<TextureData>(texture);
	m_Pivot = { 0,0 };
	m_Size = { 1,1 };
}

void TextureMaterial::LoadEffectVariables()
{
	m_pColorVariable = GetEffect()->GetVariableByName("gColor")->AsVector();
	if (!m_pColorVariable) Logger::LogWarning(L"TextureMaterial::LoadEffectVariables >> Failed to find m_pColorVariable");
	m_pPivotVariable = GetEffect()->GetVariableByName("gPivot")->AsVector();
	if (!m_pPivotVariable) Logger::LogWarning(L"TextureMaterial::LoadEffectVariables >> Failed to find m_pPivotVariable");
	m_pSizeVariable = GetEffect()->GetVariableByName("gSize")->AsVector();
	if (!m_pSizeVariable) Logger::LogWarning(L"TextureMaterial::LoadEffectVariables >> Failed to find m_pSizeVariable");
	m_pTextureVariable = GetEffect()->GetVariableByName("gTexture")->AsShaderResource();
	if (!m_pTextureVariable) Logger::LogWarning(L"TextureMaterial::LoadEffectVariables >> Failed to find m_pTextureVariable");
	m_pTextureSizeVariable = GetEffect()->GetVariableByName("gTextureSize")->AsVector();
	if (!m_pTextureSizeVariable) Logger::LogWarning(L"TextureMaterial::LoadEffectVariables >> Failed to find m_pTextureSizeVariable");


}

void TextureMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pColorVariable)
	{
		m_pColorVariable->SetFloatVector((float*)&m_Color);
	}
	if (m_pPivotVariable)
	{
		m_pPivotVariable->SetFloatVector((float*)&m_Pivot);
	}
	if (m_pSizeVariable)
	{
		m_pSizeVariable->SetFloatVector((float*)&m_Size);
	}
	if (m_pTextureVariable && m_pTexture)
	{
		m_pTextureVariable->SetResource(m_pTexture->GetShaderResourceView());
	}
	if (m_pTextureSizeVariable && m_pTexture)
	{
		const auto& dim = m_pTexture->GetDimension();
		m_pTextureSizeVariable->SetFloatVector((float*)&dim);
	}
}

