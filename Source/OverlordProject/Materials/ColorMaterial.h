#pragma once
#include "Material.h"
class TextureData;
class ColorMaterial abstract: public Material
{
public:
	ColorMaterial(const std::wstring& effectFile, const std::wstring& technique = L"",
		const DirectX::XMFLOAT4& color = { 1,1,1,1 }, const std::string& colorName = "gColor",
		const std::string& textureName = "gTexture");
	virtual ~ColorMaterial() = default;
	ColorMaterial(const ColorMaterial &obj) = delete;
	ColorMaterial& operator=(const ColorMaterial& obj) = delete;
	ColorMaterial(ColorMaterial&& obj) noexcept = delete;
	ColorMaterial& operator=(ColorMaterial&& obj) noexcept = delete;

	void SetTexture(const std::wstring& path);
	void ClearTexture() { m_pTexture = nullptr; }
	void SetColor(const DirectX::XMFLOAT4& color) { m_Color = color; }
	const DirectX::XMFLOAT4& GetColor() const { return m_Color; }

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);
private:
	std::string m_TextureName;
	std::string m_ColorName;
	DirectX::XMFLOAT4 m_Color = { 1,1,1,1 };
	TextureData* m_pTexture;

	ID3DX11EffectShaderResourceVariable* m_pTextureVariable;
	ID3DX11EffectVectorVariable* m_pColorVariable;
};

