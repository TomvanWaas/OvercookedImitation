#pragma once
#include "PostProcessingMaterial.h"

class TextureData;
class FirePostMaterial final : public PostProcessingMaterial
{
public:
	explicit FirePostMaterial();
	virtual ~FirePostMaterial() = default;

	void SetMaskSpeed(const DirectX::XMFLOAT2& speed) { m_MaskSpeed = speed; }
	const DirectX::XMFLOAT2& GetMaskSpeed() const { return m_MaskSpeed; }
	void SetDiffuseSpeed01(const DirectX::XMFLOAT2& speed) { m_DiffuseSpeed01 = speed; }
	const DirectX::XMFLOAT2& GetDiffuseSpeed01() const { return m_DiffuseSpeed01; }
	void SetDiffuseSpeed02(const DirectX::XMFLOAT2& speed) { m_DiffuseSpeed02 = speed; }
	const DirectX::XMFLOAT2& GetDiffuseSpeed02() const { return m_DiffuseSpeed02; }
	void SetTime(float time) { m_Time = time; }
	float GetTime() const { return m_Time; }
	void SetIntensity(float intensity) { m_Intensity = intensity; }
	float GetIntensity() const { return m_Intensity; }
	void SetDiffuseTexture01(const std::wstring& path);
	void SetDiffuseTexture02(const std::wstring& path);
	void SetMaskTextureEdges(const std::wstring& path);
	void SetMaskTextureFull(const std::wstring& path);

protected:
	virtual void LoadEffectVariables() override;
	virtual void UpdateEffectVariables(RenderTarget* rendertarget) override;
private:
	DirectX::XMFLOAT2 m_MaskSpeed;
	DirectX::XMFLOAT2 m_DiffuseSpeed01;
	DirectX::XMFLOAT2 m_DiffuseSpeed02;
	float m_Time;
	float m_Intensity;
	TextureData* m_pDiffuseTexture01;
	TextureData* m_pDiffuseTexture02;
	TextureData* m_pMaskTextureEdges;
	TextureData* m_pMaskTextureFull;

	static ID3DX11EffectVectorVariable* m_pMaskSpeedVariable;
	static ID3DX11EffectVectorVariable* m_pDiffuseSpeed01Variable;
	static ID3DX11EffectVectorVariable* m_pDiffuseSpeed02Variable;
	static ID3DX11EffectScalarVariable* m_pTimeVariable;
	static ID3DX11EffectScalarVariable* m_pIntensityVariable;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseTexture01Variable;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseTexture02Variable;
	static ID3DX11EffectShaderResourceVariable* m_pMaskTextureEdgesVariable;
	static ID3DX11EffectShaderResourceVariable* m_pMaskTextureFullVariable;
	static ID3DX11EffectShaderResourceVariable* m_pTextureMapVariable;

};

