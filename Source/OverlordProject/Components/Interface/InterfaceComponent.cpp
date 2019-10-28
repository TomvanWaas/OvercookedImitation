#include "stdafx.h"
#include "InterfaceComponent.h"
#include "ContentManager.h"
#include "EffectHelper.h"
#include "TextureData.h"
#include "TransformComponent.h"
#include "OverlordGame.h"
#include "GameScene.h"

ID3DX11EffectVectorVariable* InterfaceComponent::m_pPivotVariable = nullptr;
ID3DX11EffectVectorVariable* InterfaceComponent::m_pSizeVariable = nullptr;
ID3DX11EffectVectorVariable* InterfaceComponent::m_pTextureSizeVariable = nullptr;
ID3DX11EffectVectorVariable* InterfaceComponent::m_pColorVariable = nullptr;
ID3DX11EffectVectorVariable* InterfaceComponent::m_pScreenSizeVariable = nullptr;
ID3DX11EffectShaderResourceVariable* InterfaceComponent::m_pTextureVariable = nullptr;

InterfaceComponent::InterfaceComponent(const std::wstring& texture, const DirectX::XMFLOAT2& pivot,
	const DirectX::XMFLOAT2& size, const DirectX::XMFLOAT4& color, float rotation)
	: m_pTexture(ContentManager::Load<TextureData>(texture))
	, m_Pivot(pivot)
	, m_Size(size)
	, m_Vertex({}, { 1,1 }, rotation)
	, m_Color(color)
{
}

InterfaceComponent::~InterfaceComponent()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
}

void InterfaceComponent::SetTexture(const std::wstring& texture)
{
	m_pTexture = ContentManager::Load<TextureData>(texture);
	m_Pivot = { 0,0 };
	m_Size = { 1,1 };
}


void InterfaceComponent::Initialize(const GameContext& gameContext)
{
	LoadEffect(gameContext);
	CreateVertexBuffer(gameContext);
}
void InterfaceComponent::Update(const GameContext& gameContext)
{
	const auto* pTransform = GetTransform();
	const auto& wpos = pTransform->GetWorldPosition();
	m_Vertex.Position = wpos;
	const auto& wscale = pTransform->GetWorldScale();
	m_Vertex.Scale = { wscale.x, wscale.y };


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	TextureVertex* pVertex = (TextureVertex*)mappedResource.pData;
	pVertex[0] = m_Vertex;
	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}
void InterfaceComponent::Draw(const GameContext&)
{
}
void InterfaceComponent::PostDraw(const GameContext& gameContext)
{
	DrawParticle(gameContext);
}


void InterfaceComponent::LoadEffect(const GameContext& gameContext)
{
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/InterfaceEffect.fx");

	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, m_InputLayoutSize);

	//WVP
	m_pWvpVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pWvpVariable) Logger::LogWarning(L"InterfaceComponent::LoadEffect >> Failed to find m_pWvpVariable");
	//ViewInverse
	m_pViewInverseVariable = m_pEffect->GetVariableByName("gViewInverse")->AsMatrix();
	if (!m_pViewInverseVariable) Logger::LogWarning(L"InterfaceComponent::LoadEffect >> Failed to find m_pViewInverseVariable");
	//Pivot
	m_pPivotVariable = m_pEffect->GetVariableByName("gPivot")->AsVector();
	if (!m_pPivotVariable) Logger::LogWarning(L"InterfaceComponent::LoadEffect >> Failed to find m_pPivotVariable");
	//Size
	m_pSizeVariable = m_pEffect->GetVariableByName("gSize")->AsVector();
	if (!m_pSizeVariable) Logger::LogWarning(L"InterfaceComponent::LoadEffect >> Failed to find m_pSizeVariable");
	//Color
	m_pColorVariable = m_pEffect->GetVariableByName("gColor")->AsVector();
	if (!m_pColorVariable) Logger::LogWarning(L"InterfaceComponent::LoadEffect >> Failed to find m_pColorVariable");
	//Texture
	m_pTextureVariable = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
	if (!m_pTextureVariable) Logger::LogWarning(L"InterfaceComponent::LoadEffect >> Failed to find m_pTextureVariable");
	//TextureSize
	m_pTextureSizeVariable = m_pEffect->GetVariableByName("gTextureSize")->AsVector();
	if (!m_pTextureSizeVariable) Logger::LogWarning(L"InterfaceComponent::LoadEffect >> Failed to find m_pTextureSizeVariable");
	//ScreenSize
	m_pScreenSizeVariable = m_pEffect->GetVariableByName("gScreenSize")->AsVector();
	if (!m_pScreenSizeVariable) Logger::LogWarning(L"InterfaceComponent::LoadEffect >> Failed to find m_pScreenSizeVariable");
}
void InterfaceComponent::CreateVertexBuffer(const GameContext& gameContext)
{
	SafeRelease(m_pVertexBuffer);

	D3D11_BUFFER_DESC desc{};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(TextureVertex);
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	if (FAILED(gameContext.pDevice->CreateBuffer(&desc, nullptr, &m_pVertexBuffer)))
	{
		Logger::LogError(L"ParticleComponent::CreateVertexBuffer > Failed to create vertexBuffer");
	}
}

void InterfaceComponent::DrawParticle(const GameContext& gameContext)
{
	//WVP
	if (m_pWvpVariable)
	{
		m_pWvpVariable->SetMatrix((const float*)&gameContext.pCamera->GetViewProjection());
	}
	//ViewInverse
	if (m_pViewInverseVariable)
	{
		m_pViewInverseVariable->SetMatrix((const float*)&gameContext.pCamera->GetViewInverse());
	}
	//Pivot
	if (m_pPivotVariable)
	{
		m_pPivotVariable->SetFloatVector((float*)&m_Pivot);
	}
	//Size
	if (m_pSizeVariable)
	{
		m_pSizeVariable->SetFloatVector((float*)&m_Size);
	}
	//Color
	if (m_pColorVariable)
	{
		m_pColorVariable->SetFloatVector((float*)&m_Color);
	}
	//Texture
	if (m_pTextureVariable && m_pTexture)
	{
		m_pTextureVariable->SetResource(m_pTexture->GetShaderResourceView());
	}
	//TextureSize
	if (m_pTextureSizeVariable && m_pTexture)
	{
		const auto& dim = m_pTexture->GetDimension();
		m_pTextureSizeVariable->SetFloatVector((float*)&dim);
	}
	//ScreenSize
	if (m_pScreenSizeVariable)
	{
		const GameSettings& settings = OverlordGame::GetGameSettings();
		DirectX::XMFLOAT2 screenSize{ float(settings.Window.Width), float(settings.Window.Height) };
		m_pScreenSizeVariable->SetFloatVector((float*)&screenSize);
	}


	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	UINT stride = sizeof(TextureVertex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (unsigned int j = 0; j < techDesc.Passes; ++j)
	{
		m_pTechnique->GetPassByIndex(j)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(1, 0);
	}
}

