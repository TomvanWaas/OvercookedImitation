#include "stdafx.h"
#include "ExtinguisherComponent.h"
#include "GameScene.h"
#include "GameObject.h"
#include "../Other/FireComponent.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "../../Materials/ColorMaterial.h"

#ifdef _DEBUG
#include "DebugRenderer.h"
#endif

ExtinguisherComponent::ExtinguisherComponent(float range, float angle, float speed)
	: m_Range(range)
	, m_Angle(angle)
	, m_Speed(speed)
	, m_pMaterial(nullptr)
	, m_IsHighlighted(false)
	, m_pFires()
{
}

void ExtinguisherComponent::Highlight(const DirectX::XMFLOAT4& color)
{
	m_IsHighlighted = true;
	if (m_pMaterial)
	{
		m_pMaterial->SetColor(color);
	}
}

bool ExtinguisherComponent::Use(const GameContext& gameContext)
{
#ifdef _DEBUG
	{
		//Draw Area
		const TransformComponent* pT = GetTransform();
		DirectX::XMVECTOR upV = DirectX::XMLoadFloat3(&pT->GetUp());
		DirectX::XMFLOAT3 wp = pT->GetWorldPosition();

		DirectX::XMVECTOR leftV = DirectX::XMLoadFloat3(&pT->GetForward());
		leftV = DirectX::XMVector3Rotate(leftV, DirectX::XMQuaternionRotationAxis(upV, DirectX::XMConvertToRadians(-m_Angle * 0.5f)));
		DirectX::XMFLOAT3 left{};
		DirectX::XMStoreFloat3(&left, leftV);
		left.x = wp.x + left.x * m_Range;
		left.y = wp.y + left.y * m_Range;
		left.z = wp.z + left.z * m_Range;

		DirectX::XMVECTOR rightV = DirectX::XMLoadFloat3(&pT->GetForward());
		rightV = DirectX::XMVector3Rotate(rightV, DirectX::XMQuaternionRotationAxis(upV, DirectX::XMConvertToRadians(m_Angle * 0.5f)));
		DirectX::XMFLOAT3 right{};
		DirectX::XMStoreFloat3(&right, rightV);
		right.x = wp.x + right.x * m_Range;
		right.y = wp.y + right.y * m_Range;
		right.z = wp.z + right.z * m_Range;

		DebugRenderer::DrawLine(wp, left, { 1,1,1,1 });
		DebugRenderer::DrawLine(wp, right, { 1,1,1,1 });
	}
#endif

	const float elapsed = gameContext.pGameTime->GetElapsed() * m_Speed;
	for (size_t i = 0, j = m_pFires.size(); i < j; ++i)
	{
		FireComponent* pFire = m_pFires[i];
		if (pFire && InArea(*pFire))
		{
			pFire->Extinguish(elapsed);
#ifdef _DEBUG
			const float s = 12.0f;
			DirectX::XMFLOAT3 wp = pFire->GetTransform()->GetWorldPosition();
			DirectX::XMFLOAT3 ep = pFire->GetTransform()->GetWorldPosition();
			DirectX::XMFLOAT3 up = pFire->GetTransform()->GetUp();
			ep.x += up.x * s;
			ep.y += up.y * s;
			ep.z += up.z * s;
			DebugRenderer::DrawLine(wp, ep, { 0,1,0,1 });
#endif
		}
#ifdef _DEBUG
		else
		{
			const float s = 12.0f;
			DirectX::XMFLOAT3 wp = pFire->GetTransform()->GetWorldPosition();
			DirectX::XMFLOAT3 ep = pFire->GetTransform()->GetWorldPosition();
			DirectX::XMFLOAT3 up = pFire->GetTransform()->GetUp();
			ep.x += up.x * s;
			ep.y += up.y * s;
			ep.z += up.z * s;
			DebugRenderer::DrawLine(wp, ep, { 1,0,0,1 });
		}
#endif
	}
	if (m_pParticleEmitter)
	{
		m_pParticleEmitter->SetActiveSpawn(true);
	}
	return true;
}

void ExtinguisherComponent::Initialize(const GameContext& gameContext)
{
	GameObject* pObject = GetGameObject();

	m_pFires = pObject->GetScene()->GetDynamicObjectsWith<FireComponent>(true);

	//Model Material
	auto* pModelComponent = pObject->GetComponent<ModelComponent>();
	if (pModelComponent)
	{
		UINT matID = pModelComponent->GetMaterialID();
		m_pMaterial = static_cast<ColorMaterial*>(gameContext.pMaterialManager->GetMaterial(matID));
	}

	//ParticleEmitter
	GameObject* pParticlePivot = new GameObject();
	m_pParticleEmitter = new ParticleEmitterComponent(L"./Resources/Textures/Particles/Particle_Extinguish.png", 100);
	m_pParticleEmitter->SetActiveSpawn(false);
	m_pParticleEmitter->SetColor({ 1,1,1,1 });
	m_pParticleEmitter->SetMaxSize(2);
	m_pParticleEmitter->SetMinSize(1);
	m_pParticleEmitter->SetMaxEmitterRange(0);
	m_pParticleEmitter->SetMinEmitterRange(0);
	m_pParticleEmitter->SetMaxEnergy(0.75f);
	m_pParticleEmitter->SetMinEnergy(0.25f);
	m_pParticleEmitter->SetMinSizeGrow(3);
	m_pParticleEmitter->SetMaxSizeGrow(5);
	pParticlePivot->AddComponent(m_pParticleEmitter);

	pParticlePivot->GetTransform()->Rotate(90, 0, 0, true);
	pParticlePivot->GetTransform()->Translate(0, 3, 1.5f);

	pObject->AddChild(pParticlePivot);
}

void ExtinguisherComponent::Update(const GameContext&)
{
	if (m_pParticleEmitter)
	{
		//Forward Rotate random between [-angle*0.5f, angle*0.5f]

		const float strength = 30.0f;
		const TransformComponent* pTransform = GetTransform();

		DirectX::XMVECTOR forwardVector = DirectX::XMLoadFloat3(&pTransform->GetForward());
		DirectX::XMVECTOR upVector = DirectX::XMLoadFloat3(&pTransform->GetUp());
		DirectX::XMVECTOR rotationQuaternion = DirectX::XMQuaternionRotationAxis(upVector, DirectX::XMConvertToRadians(randF(-m_Angle * 0.5f, m_Angle*0.5f)));
		DirectX::XMFLOAT3 velocity{};
		DirectX::XMStoreFloat3(&velocity, DirectX::XMVector3Rotate(forwardVector, rotationQuaternion));

		m_pParticleEmitter->SetVelocity({ velocity.x * strength, velocity.y * strength, velocity.z * strength });
	}
}

void ExtinguisherComponent::PostDraw(const GameContext&)
{
	if (m_IsHighlighted && m_pMaterial)
	{
		m_pMaterial->SetColor({ 1,1,1,1 });
	}
	m_IsHighlighted = false;

	if (m_pParticleEmitter)
	{
		m_pParticleEmitter->SetActiveSpawn(false);
	}
}

bool ExtinguisherComponent::InArea(FireComponent& fire) const
{
	const TransformComponent* pTransformOther = fire.GetTransform();
	const TransformComponent* pTransformSelf = GetTransform();

	const DirectX::XMFLOAT3& wposOther = pTransformOther->GetWorldPosition();
	const DirectX::XMFLOAT3& wposSelf = pTransformSelf->GetWorldPosition();

	DirectX::XMFLOAT2 toFire{ wposOther.x - wposSelf.x, wposOther.z - wposSelf.z };
	
	//In Distance
	//***********
	if (!(toFire.x * toFire.x + toFire.y * toFire.y <= m_Range * m_Range)) return false;

	//In Angle
	//********
	
	DirectX::XMFLOAT2 forward{ pTransformSelf->GetForward().x, pTransformSelf->GetForward().z };

	DirectX::XMVECTOR toFireVector = DirectX::XMLoadFloat2(&toFire);
	DirectX::XMVECTOR forwardVector = DirectX::XMLoadFloat2(&forward);
	DirectX::XMVECTOR angleVector = DirectX::XMVector2AngleBetweenVectors(forwardVector, toFireVector);

	DirectX::XMFLOAT2 angles{};
	DirectX::XMStoreFloat2(&angles, angleVector);
	const float radians = DirectX::XMConvertToRadians(m_Angle*0.5f);
	return angles.x*angles.x <= radians*radians;
}


