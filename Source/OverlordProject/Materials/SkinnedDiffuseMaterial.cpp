#include "stdafx.h"

#include "SkinnedDiffuseMaterial.h"

#include "ContentManager.h"
#include "TextureData.h"
#include "ModelComponent.h"
#include "ModelAnimator.h"

ID3DX11EffectShaderResourceVariable* SkinnedDiffuseMaterial::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectMatrixVariable* SkinnedDiffuseMaterial::m_pBoneTransforms = nullptr;
ID3DX11EffectVectorVariable* SkinnedDiffuseMaterial::m_pColorVariable = nullptr;
ID3DX11EffectScalarVariable* SkinnedDiffuseMaterial::m_pUseTextureVariable = nullptr;

SkinnedDiffuseMaterial::SkinnedDiffuseMaterial() : Material(L"./Resources/Effects/PosNormTex3D_Skinned.fx"),
	m_pDiffuseTexture(nullptr)
	, m_Color({1,1,1,1})
	, m_UseDiffuseTexture(false)
{}

void SkinnedDiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	m_UseDiffuseTexture = true;
}

void SkinnedDiffuseMaterial::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = GetEffect()->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial::LoadEffectVariables() > \'gDiffuseMap\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}

	if (!m_pColorVariable)
	{
		m_pColorVariable = GetEffect()->GetVariableByName("gColor")->AsVector();
		if (!m_pColorVariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial::LoadEffectVariables() > \'gColor\' variable not found!");
			m_pColorVariable = nullptr;
		}
	}
	if (!m_pUseTextureVariable)
	{
		m_pUseTextureVariable = GetEffect()->GetVariableByName("gUseDiffuseMap")->AsScalar();
		if (!m_pUseTextureVariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial::LoadEffectVariables() > \'gUseDiffuseMap\' variable not found!");
			m_pUseTextureVariable = nullptr;
		}
	}

	//Create a link to the gBones variable
	if (!m_pBoneTransforms)
	{
		m_pBoneTransforms = GetEffect()->GetVariableByName("gBones")->AsMatrix();
		if (!m_pBoneTransforms->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial::LoadEffectVariables() > \'gBones\' variable not found!");
			m_pBoneTransforms = nullptr;
		}
	}
}

void SkinnedDiffuseMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}

	//Set the matrix array (BoneTransforms of the ModelAnimator)}
	if (m_pBoneTransforms != nullptr && pModelComponent != nullptr && pModelComponent->HasAnimator())
	{
		auto ts = pModelComponent->GetAnimator()->GetBoneTransforms();
		m_pBoneTransforms->SetMatrixArray((const float*)ts.data(), NULL, ts.size());
	}

	if (m_pColorVariable)
	{
		m_pColorVariable->SetFloatVector((float*)&m_Color);
	}
	if (m_pUseTextureVariable)
	{
		m_pUseTextureVariable->SetBool(m_UseDiffuseTexture);
	}



}