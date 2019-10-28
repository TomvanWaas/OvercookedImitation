#pragma once
#include "Material.h"

class TextureData;
class TextureMaterial final : public Material
{
public:
	explicit TextureMaterial(const std::wstring& texture, const DirectX::XMFLOAT2& pivot = { 0,0 },
		const DirectX::XMFLOAT2& size = { 1,1 }, const DirectX::XMFLOAT4& color = { 1,1,1,1 }, bool doubleSided = false);
	explicit TextureMaterial(const std::wstring& texture, bool doubleSided);
	virtual ~TextureMaterial() = default;
	TextureMaterial(const TextureMaterial& other) = delete;
	TextureMaterial(TextureMaterial&& other) noexcept = delete;
	TextureMaterial& operator=(const TextureMaterial& other) = delete;
	TextureMaterial& operator=(TextureMaterial&& other) noexcept = delete;

	void SetTexture(const std::wstring& texture);
	TextureData* GetTexture() const { return m_pTexture; }
	void SetSize(const DirectX::XMFLOAT2& size) { m_Size = size; }
	const DirectX::XMFLOAT2& GetSize() const { return m_Size; }
	void SetPivot(const DirectX::XMFLOAT2& pivot) { m_Pivot = pivot; }
	const DirectX::XMFLOAT2& GetPivot() const { return m_Pivot; }
	void SetColor(const DirectX::XMFLOAT4& color) { m_Color = color; }
	const DirectX::XMFLOAT4& GetColor() const { return m_Color; }
	

protected:
	virtual void LoadEffectVariables() override;
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;
private:
	DirectX::XMFLOAT2 m_Size;
	static ID3DX11EffectVectorVariable* m_pSizeVariable;

	DirectX::XMFLOAT2 m_Pivot;
	static ID3DX11EffectVectorVariable* m_pPivotVariable;

	DirectX::XMFLOAT4 m_Color;
	static ID3DX11EffectVectorVariable* m_pColorVariable;

	TextureData* m_pTexture;
	static ID3DX11EffectShaderResourceVariable* m_pTextureVariable;
	static ID3DX11EffectVectorVariable* m_pTextureSizeVariable;

};

