#include "stdafx.h"
#include "FirePostMaterial.h"
#include "TextureData.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "ContentManager.h"
#include "RenderTarget.h"

ID3DX11EffectVectorVariable*FirePostMaterial::m_pMaskSpeedVariable = nullptr;
ID3DX11EffectVectorVariable*FirePostMaterial::m_pDiffuseSpeed01Variable = nullptr;
ID3DX11EffectVectorVariable*FirePostMaterial::m_pDiffuseSpeed02Variable = nullptr;
ID3DX11EffectScalarVariable*FirePostMaterial::m_pTimeVariable = nullptr;
ID3DX11EffectShaderResourceVariable* FirePostMaterial::m_pDiffuseTexture01Variable = nullptr;
ID3DX11EffectShaderResourceVariable* FirePostMaterial::m_pDiffuseTexture02Variable = nullptr;
ID3DX11EffectShaderResourceVariable* FirePostMaterial::m_pMaskTextureEdgesVariable = nullptr;
ID3DX11EffectShaderResourceVariable* FirePostMaterial::m_pMaskTextureFullVariable = nullptr;
ID3DX11EffectShaderResourceVariable* FirePostMaterial::m_pTextureMapVariable = nullptr;
ID3DX11EffectScalarVariable* FirePostMaterial::m_pIntensityVariable = nullptr;

FirePostMaterial::FirePostMaterial()
	: PostProcessingMaterial(L"./Resources/Effects/Post/FireEffect.fx")
	, m_Time(0)
	, m_Intensity(1.0f)
	, m_MaskSpeed({0,0.5f})
	, m_DiffuseSpeed01(1, 1)
	, m_DiffuseSpeed02(-3,4)
	, m_pDiffuseTexture01(ContentManager::Load<TextureData>(L"./Resources/Textures/Effects/FireTexture01.png"))
	, m_pDiffuseTexture02(ContentManager::Load<TextureData>(L"./Resources/Textures/Effects/FireTexture02.png"))
	, m_pMaskTextureFull(ContentManager::Load<TextureData>(L"./Resources/Textures/Effects/FireMask01.png"))
	, m_pMaskTextureEdges(ContentManager::Load<TextureData>(L"./Resources/Textures/Effects/FireMask02.png"))
{
}

void FirePostMaterial::SetDiffuseTexture01(const std::wstring& path)
{
	m_pDiffuseTexture01 = ContentManager::Load<TextureData>(path);
}
void FirePostMaterial::SetDiffuseTexture02(const std::wstring& path)
{
	m_pDiffuseTexture02 = ContentManager::Load<TextureData>(path);
}
void FirePostMaterial::SetMaskTextureEdges(const std::wstring& path)
{
	m_pMaskTextureEdges = ContentManager::Load<TextureData>(path);
}
void FirePostMaterial::SetMaskTextureFull(const std::wstring& path)
{
	m_pMaskTextureFull = ContentManager::Load<TextureData>(path);
}

void FirePostMaterial::LoadEffectVariables()
{
	m_pMaskSpeedVariable = GetEffect()->GetVariableByName("gMaskSpeed")->AsVector();
	m_pDiffuseSpeed01Variable = GetEffect()->GetVariableByName("gDiffuseSpeed01")->AsVector();
	m_pDiffuseSpeed02Variable = GetEffect()->GetVariableByName("gDiffuseSpeed02")->AsVector();
	m_pTimeVariable = GetEffect()->GetVariableByName("gTime")->AsScalar();
	m_pDiffuseTexture01Variable = GetEffect()->GetVariableByName("gDiffuseTexture01")->AsShaderResource();
	m_pDiffuseTexture02Variable = GetEffect()->GetVariableByName("gDiffuseTexture02")->AsShaderResource();
	m_pMaskTextureEdgesVariable = GetEffect()->GetVariableByName("gMaskTextureEdges")->AsShaderResource();
	m_pMaskTextureFullVariable = GetEffect()->GetVariableByName("gMaskTextureFull")->AsShaderResource();
	m_pTextureMapVariable = GetEffect()->GetVariableByName("gTexture")->AsShaderResource();
	m_pIntensityVariable = GetEffect()->GetVariableByName("gIntensity")->AsScalar();
}
void FirePostMaterial::UpdateEffectVariables(RenderTarget* pRendertarget)
{
	if (m_pTextureMapVariable != nullptr && pRendertarget != nullptr)
	{
		m_pTextureMapVariable->SetResource(pRendertarget->GetShaderResourceView());
	}

	auto* pTime = SceneManager::GetInstance()->GetActiveScene()->GetGameContext().pGameTime;
	m_Time += pTime->GetElapsed() * int(pTime->IsRunning());

	if (m_pMaskSpeedVariable) m_pMaskSpeedVariable->SetFloatVector((float*)&m_MaskSpeed);
	if (m_pDiffuseSpeed01Variable) m_pDiffuseSpeed01Variable->SetFloatVector((float*)&m_DiffuseSpeed01);
	if (m_pDiffuseSpeed02Variable) m_pDiffuseSpeed02Variable->SetFloatVector((float*)&m_DiffuseSpeed02);
	if (m_pTimeVariable) m_pTimeVariable->SetFloat(m_Time);
	if (m_pDiffuseTexture01Variable && m_pDiffuseTexture01) m_pDiffuseTexture01Variable->SetResource(m_pDiffuseTexture01->GetShaderResourceView());
	if (m_pDiffuseTexture02Variable && m_pDiffuseTexture02) m_pDiffuseTexture02Variable->SetResource(m_pDiffuseTexture02->GetShaderResourceView());
	if (m_pMaskTextureEdgesVariable && m_pMaskTextureEdges) m_pMaskTextureEdgesVariable->SetResource(m_pMaskTextureEdges->GetShaderResourceView());
	if (m_pMaskTextureFullVariable && m_pMaskTextureFull) m_pMaskTextureFullVariable->SetResource(m_pMaskTextureFull->GetShaderResourceView());
	if (m_pIntensityVariable) m_pIntensityVariable->SetFloat(m_Intensity);

}

