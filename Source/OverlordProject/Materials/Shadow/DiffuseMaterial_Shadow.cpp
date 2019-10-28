//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "DiffuseMaterial_Shadow.h"
#include "GeneralStructs.h"
#include "Logger.h"
#include "Components.h"

ID3DX11EffectShaderResourceVariable* DiffuseMaterial_Shadow::m_pShadowSRVvariable = nullptr;
ID3DX11EffectVectorVariable* DiffuseMaterial_Shadow::m_pLightDirectionVariable = nullptr;
ID3DX11EffectMatrixVariable* DiffuseMaterial_Shadow::m_pLightWVPvariable = nullptr;

DiffuseMaterial_Shadow::DiffuseMaterial_Shadow()
: ColorMaterial(L"./Resources/Effects/Shadow/PosNormTex3D_Shadow.fx", L"", {1,1,1,1}, "gColor", "gDiffuseMap")
{}


void DiffuseMaterial_Shadow::SetLightDirection(DirectX::XMFLOAT3 dir)
{
	m_LightDirection = dir;
}

void DiffuseMaterial_Shadow::LoadEffectVariables()
{
	ColorMaterial::LoadEffectVariables();

	//load all the necessary shader variables
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
}

void DiffuseMaterial_Shadow::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	ColorMaterial::UpdateEffectVariables(gameContext, pModelComponent);

	
	//update all the necessary shader variables
	if (m_pLightDirectionVariable) m_pLightDirectionVariable->SetFloatVector((float*)&m_LightDirection);
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
	if (m_pShadowSRVvariable) m_pShadowSRVvariable->SetResource(gameContext.pShadowMapper->GetShadowMap());

}


