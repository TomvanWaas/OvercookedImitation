#pragma once
#include "../ColorMaterial.h"

class TextureData;

class SkinnedDiffuseMaterial_Shadow final : public ColorMaterial
{
public:
	explicit SkinnedDiffuseMaterial_Shadow();
	virtual ~SkinnedDiffuseMaterial_Shadow() = default;

	void SetLightDirection(const DirectX::XMFLOAT3& dir);
	void SetUseTexture(bool use) { m_UseDiffuseTexture = use; }
protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

private:
	DirectX::XMFLOAT3 m_LightDirection = { -0.577f, -0.577f, 0.577f };
	bool m_UseDiffuseTexture = false;

	static ID3DX11EffectMatrixVariable* m_pBoneTransforms;
	static ID3DX11EffectVectorVariable* m_pLightDirectionVariable;
	static ID3DX11EffectShaderResourceVariable* m_pShadowSRVvariable;
	static ID3DX11EffectMatrixVariable* m_pLightWVPvariable;
	static ID3DX11EffectScalarVariable* m_pUseDiffuseSRVVariable;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkinnedDiffuseMaterial_Shadow(const SkinnedDiffuseMaterial_Shadow &obj);
	SkinnedDiffuseMaterial_Shadow& operator=(const SkinnedDiffuseMaterial_Shadow& obj);
};

