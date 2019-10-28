#pragma once
#include "Material.h"

class TextureData;
class WaterMaterial final : public Material
{
public:
	explicit WaterMaterial();
	virtual ~WaterMaterial() = default;

	void SetTimeSpeed(float speed) { m_TimeSpeed = speed; }
	float GetTimeSpeed() const { return m_TimeSpeed; }
	void SetDeepColor(const DirectX::XMFLOAT3& color) { m_DeepColor = color; }
	const DirectX::XMFLOAT3& GetDeepColor() const { return m_DeepColor; }
	void SetTimer(float timer) { m_Timer = timer; }
	float GetTimer() const { return m_Timer; }
	void SetShallowColor(const DirectX::XMFLOAT3& color) { m_ShallowColor = color; }
	const DirectX::XMFLOAT3& GetShallowColor() const { return m_ShallowColor; }
	void SetBumpScale(float scale) { m_BumpScale = scale; }
	float GetBumpScale() const { return m_BumpScale; }
	void SetReflectionTint(const DirectX::XMFLOAT3& color) { m_ReflectionTint = color; }
	const DirectX::XMFLOAT3& GetReflectionTint() const { return m_ReflectionTint; }
	void SetTextureRepeatX(float repeat) { m_RepeatX = repeat; }
	float GetTextureRepeatX() const { return m_RepeatX; }
	void SetTextureRepeatY(float repeat) { m_RepeatY = repeat; }
	float GetTextureRepeatY() const { return m_RepeatY; }
	void SetBumpSpeedX(float speed) { m_BumpSpeedX = speed; }
	float GetBumpSpeedX() const { return m_BumpSpeedX; }
	void SetBumpSpeedY(float speed) { m_BumpSpeedY = speed; }
	float GetBumpSpeedY() const { return m_BumpSpeedY; }
	void SetFresnelBias(float bias) { m_FresnelBias = bias; }
	float GetFresnelBias() const { return m_FresnelBias; }
	void SetFresnelExponent(float exponent) { m_FresnelExponent = exponent; }
	float GetFresnelExponent() const { return m_FresnelExponent; }
	void SetHDRMultiplier(float multiplier) { m_HDRMultiplier = multiplier; }
	float GetHDRMultiplier() const { return m_HDRMultiplier; }
	void SetReflectionStrength(float strength) { m_ReflectionStrength = strength; }
	float GetReflectionStrength() const { return m_ReflectionStrength; }
	void SetWaterColorStrength(float strength) { m_WaterColorStrength = strength; }
	float GetWaterColorStrength() const { return m_WaterColorStrength; }
	void SetWaveAmplitude(float amplitude) { m_WaveAmplitude = amplitude; }
	float GetWaveAmplitude() const { return m_WaveAmplitude; }
	void SetWaveFrequency(float frequency) { m_WaveFrequency = frequency; }
	float GetWaveFrequency() const { return m_WaveFrequency; }
	void SetNormalTexture(const std::wstring& path);
	void SetEnvironmentTexture(const std::wstring& path);
protected:
	virtual void LoadEffectVariables() override;
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;
private:
	DirectX::XMFLOAT3 m_DeepColor;
	float m_Timer;
	DirectX::XMFLOAT3 m_ShallowColor;
	float m_BumpScale;
	DirectX::XMFLOAT3 m_ReflectionTint;
	float m_RepeatX;
	float m_RepeatY;
	float m_BumpSpeedX;
	float m_BumpSpeedY;
	float m_FresnelBias;
	float m_FresnelExponent;
	float m_HDRMultiplier;
	float m_ReflectionStrength;
	float m_WaterColorStrength;
	float m_WaveAmplitude;
	float m_WaveFrequency;
	float m_TimeSpeed;
	TextureData* m_pNormalTexture;
	TextureData* m_pEnvironmentTexture;



	static ID3DX11EffectScalarVariable* m_pTimerVariable;
	static ID3DX11EffectShaderResourceVariable* m_pNormalTextureVariable;
	static ID3DX11EffectShaderResourceVariable* m_pEnvironmentTextureVariable;
	static ID3DX11EffectScalarVariable* m_pBumpScaleVariable;
	static ID3DX11EffectScalarVariable* m_pRepeatXVariable;
	static ID3DX11EffectScalarVariable* m_pRepeatYVariable;
	static ID3DX11EffectScalarVariable* m_pBumpSpeedXVariable;
	static ID3DX11EffectScalarVariable* m_pBumpSpeedYVariable;
	static ID3DX11EffectScalarVariable* m_pFresnelBiasVariable;
	static ID3DX11EffectScalarVariable* m_pFresnelExponentVariable;
	static ID3DX11EffectScalarVariable* m_pHDRMultiplierVariable;
	static ID3DX11EffectVectorVariable* m_pDeepColorVariable;
	static ID3DX11EffectVectorVariable* m_pShallowColorVariable;
	static ID3DX11EffectVectorVariable* m_pReflectionTintVariable;
	static ID3DX11EffectScalarVariable* m_pReflectionStrengthVariable;
	static ID3DX11EffectScalarVariable* m_pWaterColorStrengthVariable;
	static ID3DX11EffectScalarVariable* m_pWaveAmplitudeVariable;
	static ID3DX11EffectScalarVariable* m_pWaveFrequencyVariable;

};

