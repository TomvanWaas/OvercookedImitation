#include "stdafx.h"
#include "BoxComponentFoodContainer.h"
#include "../../Singletons/AssetFiles.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include "../../Materials/TextureMaterial.h"
#include "TransformComponent.h"
#include "../../Singletons/Templates.h"
#include "../../Helpers/Helpers.h"

BoxComponentFoodContainer::BoxComponentFoodContainer(
	eBoxType type, 
	const DirectX::XMFLOAT3& holderOffset,
	const std::vector<eHolderType>& allowedHolders,
	const ModelInfo& initInfo, 
	eFoodType templateType,
	const std::function<bool(const HolderPointer& holder)>& requirement)

	: BoxComponentBase(type, holderOffset, allowedHolders, initInfo)
	, m_TemplateType(templateType)
	, m_TemplateRequirement(requirement)
{
}

bool BoxComponentFoodContainer::TakeFood(HolderPointer& other)
{
	if (m_ItemPointer.IsValid() || IsOnFire()) return false;

	//Contains Food => Base TakeFood
	//******************************
	if (!m_HolderPointer.IsEmpty()) return BoxComponentBase::TakeFood(other);

	//Else if Empty => Give Template Clone
	//************************************
	if (m_TemplateRequirement == nullptr || m_TemplateRequirement(other))
	{
		//Add Food Clone
		//**************
		Helpers::AddFood(other, m_TemplateType);
		TakeSound();
		return true;
	}
	return false;
}

void BoxComponentFoodContainer::Initialize(const GameContext& gameContext)
{
	//Plane Texture
	//*************
	std::wstring asset{};
	if (AssetFiles::GetInstance().Get(eAssetId::Model_Default_Plane, asset))
	{
		const float size = 6.0f;
		const float scale = 0.75f;

		GameObject* pChild = new GameObject();
		ModelComponent* pModel = new ModelComponent(asset);

		//Material
		//********
		if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Icons_Food, asset))
		{
			UINT materialID = gameContext.pMaterialManager->GetFreeID();
			m_pTemplateMaterial = new TextureMaterial(asset, true);

			const float offset = 1.0f / (int(eFoodType::_END));
			const float selfOffset = (int(m_TemplateType) * offset);
			m_pTemplateMaterial->SetPivot({ selfOffset, 0 });
			m_pTemplateMaterial->SetSize({ offset, 1 });

			gameContext.pMaterialManager->AddMaterial(m_pTemplateMaterial, materialID);
			pModel->SetMaterial(materialID);
		}

		pChild->AddComponent(pModel);
		auto* pTransform = pChild->GetTransform();
		pTransform->Translate(0, size, 0);
		pTransform->Scale(scale, scale, scale);
		GetGameObject()->AddChild(pChild);
	}

	BoxComponentBase::Initialize(gameContext);
}


void BoxComponentFoodContainer::EnableHighlight(const DirectX::XMFLOAT4& color)
{
	if (m_pTemplateMaterial)
	{
		m_pTemplateMaterial->SetColor(color);
	}
}
void BoxComponentFoodContainer::DisableHighlight()
{
	if (m_pTemplateMaterial)
	{
		m_pTemplateMaterial->SetColor({ 1,1,1,1 });
	}
}
