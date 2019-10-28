//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SkinnedDiffuseMaterial_Shadow.h"
#include "GeneralStructs.h"
#include "Logger.h"
#include "ContentManager.h"
#include "TextureData.h"
#include "ModelComponent.h"
#include "ModelAnimator.h"
#include "Components.h"

ID3DX11EffectMatrixVariable* SkinnedDiffuseMaterial_Shadow::m_pBoneTransforms = nullptr;
ID3DX11EffectVectorVariable* SkinnedDiffuseMaterial_Shadow::m_pLightDirectionVariable = nullptr;
ID3DX11EffectShaderResourceVariable* SkinnedDiffuseMaterial_Shadow::m_pShadowSRVvariable = nullptr;
ID3DX11EffectMatrixVariable* SkinnedDiffuseMaterial_Shadow::m_pLightWVPvariable = nullptr;
ID3DX11EffectScalarVariable* SkinnedDiffuseMaterial_Shadow::m_pUseDiffuseSRVVariable = nullptr;

SkinnedDiffuseMaterial_Shadow::SkinnedDiffuseMaterial_Shadow() 
	: ColorMaterial(L"./Resources/Effects/Shadow/PosNormTex3D_Skinned_Shadow.fx", L"",
		{1,1,1,1}, "gColor", "gDiffuseMap")

{}


void SkinnedDiffuseMaterial_Shadow::SetLightDirection(const DirectX::XMFLOAT3& dir)
{
	//UNREFERENCED_PARAMETER(dir);
	//store the light direction in the appropriate member
	m_LightDirection = dir;
}

void SkinnedDiffuseMaterial_Shadow::LoadEffectVariables()
{
	ColorMaterial::LoadEffectVariables();

	//load all the necessary shader variables

	if (!m_pBoneTransforms)
	{
		m_pBoneTransforms = GetEffect()->GetVariableByName("gBones")->AsMatrix();
		if (!m_pBoneTransforms->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gBones\' variable not found!");
			m_pBoneTransforms = nullptr;
		}
	}

	if (!m_pLightDirectionVariable)
	{
		m_pLightDirectionVariable = GetEffect()->GetVariableByName("gLightDirection")->AsVector();
		if (!m_pLightDirectionVariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gLightDirection\' variable not found!");
			m_pLightDirectionVariable = nullptr;
		}
	}

	if (!m_pLightWVPvariable)
	{
		m_pLightWVPvariable = GetEffect()->GetVariableByName("gWorldViewProj_Light")->AsMatrix();
		if (!m_pLightWVPvariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gWorldViewProj_Light\' variable not found!");
			m_pLightWVPvariable = nullptr;
		}
	}

	if (!m_pShadowSRVvariable)
	{
		m_pShadowSRVvariable = GetEffect()->GetVariableByName("gShadowMap")->AsShaderResource();
		if (!m_pShadowSRVvariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gShadowMap\' variable not found!");
			m_pShadowSRVvariable = nullptr;
		}
	}


	if (!m_pUseDiffuseSRVVariable)
	{
		m_pUseDiffuseSRVVariable = GetEffect()->GetVariableByName("gUseDiffuseMap")->AsScalar();
		if (!m_pUseDiffuseSRVVariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gUseDiffuseMap\' variable not found!");
			m_pUseDiffuseSRVVariable = nullptr;
		}
	}
}

void SkinnedDiffuseMaterial_Shadow::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	ColorMaterial::UpdateEffectVariables(gameContext, pModelComponent);

	//UNREFERENCED_PARAMETER(gameContext);
	//UNREFERENCED_PARAMETER(pModelComponent);
	//update all the necessary shader variables
	if (m_pBoneTransforms != nullptr && pModelComponent != nullptr && pModelComponent->HasAnimator())
	{
		auto ts = pModelComponent->GetAnimator()->GetBoneTransforms();
		m_pBoneTransforms->SetMatrixArray((const float*)ts.data(), NULL, ts.size());
	}
	if (m_pLightWVPvariable && pModelComponent)
	{
		auto lvp = gameContext.pShadowMapper->GetLightVP();
		auto lvpm = DirectX::XMLoadFloat4x4(&lvp);
		auto w = pModelComponent->GetTransform()->GetWorld();
		auto wm = DirectX::XMLoadFloat4x4(&w);
		auto res = wm * lvpm;
		DirectX::XMFLOAT4X4 r{};
		DirectX::XMStoreFloat4x4(&r, res);
		m_pLightWVPvariable->SetMatrix((float*)&r);
	}
	if (m_pLightDirectionVariable) m_pLightDirectionVariable->SetFloatVector((float*)&m_LightDirection);
	if (m_pShadowSRVvariable) m_pShadowSRVvariable->SetResource(gameContext.pShadowMapper->GetShadowMap());
	if (m_pUseDiffuseSRVVariable) m_pUseDiffuseSRVVariable->SetBool(m_UseDiffuseTexture);
}


