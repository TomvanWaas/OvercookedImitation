#include "stdafx.h"
#include "CameraLightComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "../../Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"
#include "../../Structs//ModelInfo.h"

CameraLightComponent::CameraLightComponent(const DirectX::XMFLOAT3& translation, const DirectX::XMFLOAT3& rotation)
	: m_Translation(translation)
	, m_Rotation(rotation)
	, m_pPivot(nullptr)
{
}

void CameraLightComponent::Initialize(const GameContext&)
{
	GameObject* pPivotObject = new GameObject();
	m_pPivot = pPivotObject->GetTransform();

	m_pPivot->Translate(m_Translation);
	m_pPivot->Rotate(m_Rotation, true);

	GetGameObject()->AddChild(pPivotObject);
}
void CameraLightComponent::Update(const GameContext& gameContext)
{
	if (m_pPivot)
	{
		const DirectX::XMFLOAT3& position = m_pPivot->GetWorldPosition();
		const DirectX::XMFLOAT3& direction = m_pPivot->GetForward();

		//ShadowMapper
		gameContext.pShadowMapper->SetLight(position, direction);

		//Materials
		MaterialManager* pMatManager = gameContext.pMaterialManager;
		pMatManager->ForEach([&direction](Material* pMaterial, UINT)
		{
			auto* pMatShadow = dynamic_cast<DiffuseMaterial_Shadow*>(pMaterial);
			if (pMatShadow)
			{
				pMatShadow->SetLightDirection(direction);
				return;
			}
			auto* pMatShadowSkinned = dynamic_cast<SkinnedDiffuseMaterial_Shadow*>(pMaterial);
			if (pMatShadowSkinned)
			{
				pMatShadowSkinned->SetLightDirection({ -direction.x, -direction.y, -direction.z });
				//return;
			}
		});

		//ModelInfo
		ModelInfo::SetLightDirection(direction);
	}
}
