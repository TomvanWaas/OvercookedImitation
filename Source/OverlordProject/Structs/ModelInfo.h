#pragma once
enum class eAssetId;
class ModelComponent;
class ColorMaterial;
struct ModelInfo final
{
	eAssetId modelID;
	eAssetId diffuseID;

	std::pair<ModelComponent*, std::pair<ColorMaterial*, UINT>> MakeDiffuseModel(const GameContext& gameContext) const;

	static void SetLightDirection(const DirectX::XMFLOAT3& lightDirection) { m_LightDirection = lightDirection; }
	static const DirectX::XMFLOAT3& GetLightDirection() { return m_LightDirection; }
private:
	static DirectX::XMFLOAT3 m_LightDirection;
};
