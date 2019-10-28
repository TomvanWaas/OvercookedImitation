#include "stdafx.h"
#include "WaterMaterial.h"
#include "TextureData.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "ContentManager.h"
#include "EffectLoader.h"

ID3DX11EffectScalarVariable* WaterMaterial::m_pTimerVariable = nullptr;
ID3DX11EffectShaderResourceVariable* WaterMaterial::m_pNormalTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* WaterMaterial::m_pEnvironmentTextureVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pBumpScaleVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pRepeatXVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pRepeatYVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pBumpSpeedXVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pBumpSpeedYVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pFresnelBiasVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pFresnelExponentVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pHDRMultiplierVariable = nullptr;
ID3DX11EffectVectorVariable* WaterMaterial::m_pDeepColorVariable = nullptr;
ID3DX11EffectVectorVariable* WaterMaterial::m_pShallowColorVariable = nullptr;
ID3DX11EffectVectorVariable* WaterMaterial::m_pReflectionTintVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pReflectionStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pWaterColorStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pWaveAmplitudeVariable = nullptr;
ID3DX11EffectScalarVariable* WaterMaterial::m_pWaveFrequencyVariable = nullptr;


WaterMaterial::WaterMaterial()
	: Material(L"./Resources/Effects/WaterEffect.fx")
	//Default Values
	, m_Timer(0.0f)
	, m_pNormalTexture(ContentManager::Load<TextureData>(L"./Resources/Textures/Effects/Ocean_Water_A.dds"))
	, m_pEnvironmentTexture(ContentManager::Load<TextureData>(L"./Resources/Textures/Effects/Ocean_Environment_A.dds"))
	, m_BumpScale(1.4f)
	, m_RepeatX(8.0f)
	, m_RepeatY(4.0f)
	, m_BumpSpeedX(-0.05f)
	, m_BumpSpeedY(0.0f)
	, m_FresnelBias(0.1f)
	, m_FresnelExponent(4.0f)
	, m_HDRMultiplier(3.0f)
	, m_DeepColor({0,0,0.1f})
	, m_ShallowColor({0,0.5f,0.5f})
	, m_ReflectionTint({1,1,1})
	, m_ReflectionStrength(1.0f)
	, m_WaterColorStrength(1.0f)
	, m_WaveAmplitude(0.05f)
	, m_WaveFrequency(3.0f)
	, m_TimeSpeed(1.0f)
{
}

void WaterMaterial::SetNormalTexture(const std::wstring& path)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(path);
}
void WaterMaterial::SetEnvironmentTexture(const std::wstring& path)
{
	m_pEnvironmentTexture = ContentManager::Load<TextureData>(path);
}


void WaterMaterial::LoadEffectVariables()
{
	m_pTimerVariable = GetEffect()->GetVariableByName("gTimer")->AsScalar();
	if (!m_pTimerVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pTimerVariable");
	m_pNormalTextureVariable = GetEffect()->GetVariableByName("gNormalTexture")->AsShaderResource();
	if (!m_pNormalTextureVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pNormalTextureVariable");
	m_pEnvironmentTextureVariable = GetEffect()->GetVariableByName("gEnvironmentTexture")->AsShaderResource();
	if (!m_pEnvironmentTextureVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pEnvironmentTextureVariable");
	m_pBumpScaleVariable = GetEffect()->GetVariableByName("gBumpScale")->AsScalar();
	if (!m_pBumpScaleVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pBumpScaleVariable");
	m_pRepeatXVariable = GetEffect()->GetVariableByName("gTextureRepeatX")->AsScalar();
	if (!m_pRepeatXVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pRepeatXVariable");
	m_pRepeatYVariable = GetEffect()->GetVariableByName("gTextureRepeatY")->AsScalar();
	if (!m_pRepeatYVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pRepeatYVariable");
	m_pBumpSpeedXVariable = GetEffect()->GetVariableByName("gBumpSpeedX")->AsScalar();
	if (!m_pBumpSpeedXVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pBumpSpeedXVariable");
	m_pBumpSpeedYVariable = GetEffect()->GetVariableByName("gBumpSpeedY")->AsScalar();
	if (!m_pBumpSpeedYVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pBumpSpeedYVariable");
	m_pFresnelBiasVariable = GetEffect()->GetVariableByName("gFresnelBias")->AsScalar();
	if (!m_pFresnelBiasVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pFresnelBiasVariable");
	m_pFresnelExponentVariable = GetEffect()->GetVariableByName("gFresnelExponent")->AsScalar();
	if (!m_pFresnelExponentVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pFresnelExponentVariable");
	m_pHDRMultiplierVariable = GetEffect()->GetVariableByName("gHDRMultiplier")->AsScalar();
	if (!m_pHDRMultiplierVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pHDRMultiplierVariable");
	m_pDeepColorVariable = GetEffect()->GetVariableByName("gDeepColor")->AsVector();
	if (!m_pDeepColorVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pDeepColorVariable");
	m_pShallowColorVariable = GetEffect()->GetVariableByName("gShallowColor")->AsVector();
	if (!m_pShallowColorVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pShallowColorVariable");
	m_pReflectionTintVariable = GetEffect()->GetVariableByName("gReflectionTint")->AsVector();
	if (!m_pReflectionTintVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pReflectionTintVariable");
	m_pReflectionStrengthVariable = GetEffect()->GetVariableByName("gReflectionStrength")->AsScalar();
	if (!m_pReflectionStrengthVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pReflectionStrengthVariable");
	m_pWaterColorStrengthVariable = GetEffect()->GetVariableByName("gWaterColorStrength")->AsScalar();
	if (!m_pWaterColorStrengthVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pWaterColorStrengthVariable");
	m_pWaveAmplitudeVariable = GetEffect()->GetVariableByName("gWaveAmplitude")->AsScalar();
	if (!m_pWaveAmplitudeVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pWaveAmplitudeVariable");
	m_pWaveFrequencyVariable = GetEffect()->GetVariableByName("gWaveFrequency")->AsScalar();
	if (!m_pWaveFrequencyVariable) Logger::LogWarning(L"WaterMaterial::LoadEffectVariables >> Failed to load m_pWaveFrequencyVariable");
}

void WaterMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent*)
{
	m_Timer += gameContext.pGameTime->GetElapsed() * m_TimeSpeed * int(gameContext.pGameTime->IsRunning());
	if (m_pTimerVariable) m_pTimerVariable->SetFloat(m_Timer);
	if (m_pNormalTextureVariable && m_pNormalTexture) m_pNormalTextureVariable->SetResource(m_pNormalTexture->GetShaderResourceView());
	if (m_pEnvironmentTextureVariable && m_pEnvironmentTexture) m_pEnvironmentTextureVariable->SetResource(m_pEnvironmentTexture->GetShaderResourceView());
	if (m_pBumpScaleVariable) m_pBumpScaleVariable->SetFloat(m_BumpScale);
	if (m_pRepeatXVariable) m_pRepeatXVariable->SetFloat(m_RepeatX);
	if (m_pRepeatYVariable) m_pRepeatYVariable->SetFloat(m_RepeatY);
	if (m_pBumpSpeedXVariable) m_pBumpSpeedXVariable->SetFloat(m_BumpSpeedX);
	if (m_pBumpSpeedYVariable) m_pBumpSpeedYVariable->SetFloat(m_BumpSpeedY);
	if (m_pFresnelBiasVariable) m_pFresnelBiasVariable->SetFloat(m_FresnelBias);
	if (m_pFresnelExponentVariable) m_pFresnelExponentVariable->SetFloat(m_FresnelExponent);
	if (m_pHDRMultiplierVariable) m_pHDRMultiplierVariable->SetFloat(m_HDRMultiplier);
	if (m_pDeepColorVariable) m_pDeepColorVariable->SetFloatVector((float*)&m_DeepColor);
	if (m_pShallowColorVariable) m_pShallowColorVariable->SetFloatVector((float*)&m_ShallowColor);
	if (m_pReflectionTintVariable) m_pReflectionTintVariable->SetFloatVector((float*)&m_ReflectionTint);
	if (m_pReflectionStrengthVariable) m_pReflectionStrengthVariable->SetFloat(m_ReflectionStrength);
	if (m_pWaterColorStrengthVariable) m_pWaterColorStrengthVariable->SetFloat(m_WaterColorStrength);
	if (m_pWaveAmplitudeVariable) m_pWaveAmplitudeVariable->SetFloat(m_WaveAmplitude);
	if (m_pWaveFrequencyVariable) m_pWaveFrequencyVariable->SetFloat(m_WaveFrequency);
}


