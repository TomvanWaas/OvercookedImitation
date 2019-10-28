#pragma once

struct TextureVertex
{
	TextureVertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& scale, float rotation)
		: Position(position)
		, Rotation(rotation)
		, Scale(scale)
	{}

	DirectX::XMFLOAT3 Position = { 0,0,0 };
	DirectX::XMFLOAT2 Scale = { 1,1 };
	float Rotation = 0.0f;
};

class TextureData;
class InterfaceComponent : public BaseComponent
{
public:
	explicit InterfaceComponent(const std::wstring& texture,
		const DirectX::XMFLOAT2& pivot = { 0,0 },
		const DirectX::XMFLOAT2& size = { 1,1 },
		const DirectX::XMFLOAT4& color = { 1,1,1,1 },
		float rotation = 0.0f);
	virtual ~InterfaceComponent();

	void SetColor(const DirectX::XMFLOAT4& color) { m_Color = color; }
	const DirectX::XMFLOAT4& GetColor() const { return m_Color; }
	void SetRotation(float rotation) { m_Vertex.Rotation = rotation; }
	float GetRotation() const { return m_Vertex.Rotation; }
	void SetTexture(const std::wstring& texture);
	const TextureData* GetTexture() const { return m_pTexture; }
	void SetSize(const DirectX::XMFLOAT2& size) { m_Size = size; }
	const DirectX::XMFLOAT2& GetSize() const { return m_Size; }
	void SetPivot(const DirectX::XMFLOAT2& pivot) { m_Pivot = pivot; }
	const DirectX::XMFLOAT2& GetPivot() const { return m_Pivot; }

protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
	virtual void PostDraw(const GameContext& gameContext) override;
private:
	void LoadEffect(const GameContext& gameContext);
	void CreateVertexBuffer(const GameContext& gameContext);

	TextureVertex m_Vertex;
	DirectX::XMFLOAT4 m_Color;
	DirectX::XMFLOAT2 m_Size;
	DirectX::XMFLOAT2 m_Pivot;

	UINT m_InputLayoutSize;

	TextureData* m_pTexture;
	ID3DX11Effect* m_pEffect = nullptr;
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	ID3DX11EffectTechnique* m_pTechnique = nullptr;
	ID3DX11EffectMatrixVariable* m_pWvpVariable = nullptr;
	ID3DX11EffectMatrixVariable* m_pViewInverseVariable = nullptr;

	static ID3DX11EffectVectorVariable* m_pColorVariable;
	static ID3DX11EffectVectorVariable* m_pSizeVariable;
	static ID3DX11EffectVectorVariable* m_pPivotVariable;
	static ID3DX11EffectVectorVariable* m_pTextureSizeVariable;
	static ID3DX11EffectVectorVariable* m_pScreenSizeVariable;
	static ID3DX11EffectShaderResourceVariable* m_pTextureVariable;


	void DrawParticle(const GameContext& gameContext);
};

