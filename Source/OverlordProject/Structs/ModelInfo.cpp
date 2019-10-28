#include "stdafx.h"
#include "ModelInfo.h"
#include "../Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "ModelComponent.h"
#include "../Singletons/AssetFiles.h"
#include "../Materials/DiffuseMaterial.h"

DirectX::XMFLOAT3 ModelInfo::m_LightDirection{ -0.577f, -0.577f, 0.577f };
//DirectX::XMFLOAT3 ModelInfo::m_LightDirection{ 0, -1, 0 };

std::pair<ModelComponent*, std::pair<ColorMaterial*, UINT>> ModelInfo::MakeDiffuseModel(const GameContext& gameContext) const
{
	std::wstring asset{};
	std::pair<ModelComponent*, std::pair<ColorMaterial*, UINT>> result{ nullptr, {nullptr, 0} };
	UINT matIndex = 0;
	bool validIndex = false;

	//Material
	//********
	if (AssetFiles::GetInstance().Get(diffuseID, asset) && asset != L"")
	{
		MaterialManager* pManager = gameContext.pMaterialManager;
		DiffuseMaterial_Shadow* pMaterial = new DiffuseMaterial_Shadow();
		pMaterial->SetTexture(asset);
		pMaterial->SetLightDirection(m_LightDirection);
		
		matIndex = pManager->GetFreeID();
		validIndex = true;
		pManager->AddMaterial(pMaterial, matIndex);
		result.second.first = pMaterial;
		result.second.second = matIndex;
	}

	//Model
	//*****
	if (AssetFiles::GetInstance().Get(modelID, asset) && asset != L"")
	{
		ModelComponent* pModel = new ModelComponent(asset);
		if (validIndex) pModel->SetMaterial(matIndex);
		result.first = pModel;
	}

	return result;
}
