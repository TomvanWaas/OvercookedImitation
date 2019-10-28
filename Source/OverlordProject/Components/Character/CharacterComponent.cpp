#include "stdafx.h"
#include "CharacterComponent.h"
#include "PhysxManager.h"
#include "Components.h"
#include "GameObject.h"
#include "../../Helpers/Raycast.h"
#include "../../Singletons/AssetFiles.h"
#include "../../Materials/SkinnedDiffuseMaterial.h"
#include "ModelAnimator.h"
#include "../../Helpers/Helpers.h"
#include "../Box/BoxComponentBase.h"
#include "../Holder/HolderInterfaceComponent.h"
#include "../../Components/Item/ItemInterfaceComponent.h"
#include "../../Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"
#include "SceneManager.h"

CharacterComponent::CharacterComponent(GamepadIndex playerID, const DirectX::XMFLOAT4 color, 
	float radius, float height, float moveSpeed, float rotateSpeed, const DirectX::XMFLOAT3& holderOffset)
	: m_PlayerID(playerID)
	, m_Radius(radius)
	, m_Height(height)
	, m_MoveSpeed(moveSpeed)
	, m_RotateSpeed(rotateSpeed)
//	, m_pControllerComponent(nullptr)
	, m_MoveLerp(10)
	, m_ObjectOffset(holderOffset)
	, m_Velocity(1,1,1)
	, m_Color(color)
	, m_pAnimatedModel(nullptr)
	, m_Animation()
{
}

void CharacterComponent::PostInitialize(const GameContext&)
{
}
void CharacterComponent::Initialize(const GameContext& gameContext)
{
	auto* pObject = GetGameObject();

	//PxMaterial
	physx::PxMaterial* pMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(1, 1, 0);

	//ParticleEmitter
	std::wstring particle{};
	if (AssetFiles::GetInstance().Get(eAssetId::Particle_Dust, particle))
	{
		m_pParticleComponent = new ParticleEmitterComponent(particle, 5);
		m_pParticleComponent->SetMinEnergy(0.25f);
		m_pParticleComponent->SetMaxEnergy(0.5f);
		pObject->AddComponent(m_pParticleComponent);
		m_pParticleComponent->SetActiveSpawn(false);
		m_pParticleComponent->SetColor({ 1,1,1,1 });
		m_pParticleComponent->SetMaxEmitterRange(0);
		m_pParticleComponent->SetMinEmitterRange(0);
		m_pParticleComponent->SetMaxSize(1.1f);
		m_pParticleComponent->SetMinSize(0.9f);
		m_pParticleComponent->SetMaxSizeGrow(-0.5f);
		m_pParticleComponent->SetMinSizeGrow(-1);
		
	}

	////Controller
	//m_pControllerComponent = new ControllerComponent(pMaterial, m_Radius, m_Height, L"Player" + m_PlayerID, physx::PxCapsuleClimbingMode::eCONSTRAINED);
	//pObject->AddComponent(m_pControllerComponent);

	//Register Input
	auto pInput = gameContext.pInput;
	pInput->AddInputAction(InputAction(eCharacterInput::INTERACT + eCharacterInput::_END * m_PlayerID, InputTriggerState::Down, 'Q', -1, XINPUT_GAMEPAD_X, m_PlayerID));
	pInput->AddInputAction(InputAction(eCharacterInput::PLACE + eCharacterInput::_END * m_PlayerID, InputTriggerState::Pressed, 'E', -1, XINPUT_GAMEPAD_A, m_PlayerID));
	pInput->AddInputAction(InputAction(eCharacterInput::PAUSE + eCharacterInput::_END * m_PlayerID, InputTriggerState::Pressed, VK_ESCAPE, -1, XINPUT_GAMEPAD_START, m_PlayerID));

	//Add Empty Holder
	m_HolderPointer.Initialize(GetGameObject(), m_ObjectOffset);
	Helpers::AddHolder(m_HolderPointer, eHolderType::Empty);
	m_ItemPointer.Initialize(GetGameObject(), m_ObjectOffset);

	//Model (As Child (Transform Reasons))
	std::wstring asset{};
	if (AssetFiles::GetInstance().Get(eAssetId::Animated_Character, asset))
	{
		const float quo = 0.3f;
		const float scale = m_Height * quo;

		GameObject* pChild = new GameObject();
		m_pAnimatedModel = new ModelComponent(asset);

		UINT materialID = gameContext.pMaterialManager->GetFreeID();
		auto* pModelMaterial = new SkinnedDiffuseMaterial_Shadow();
		pModelMaterial->SetColor(m_Color);
		pModelMaterial->ClearTexture();
		pModelMaterial->SetUseTexture(false);
		gameContext.pMaterialManager->AddMaterial(pModelMaterial, materialID);
		m_pAnimatedModel->SetMaterial(materialID);

		pChild->GetTransform()->Rotate(0, 180, 0, true);
		pChild->GetTransform()->Scale(scale, scale, scale);

		pChild->AddComponent(m_pAnimatedModel);
		pObject->AddChild(pChild);
	}


	//RigidBody
	m_pRigidBody = new RigidBodyComponent(false);

	pObject->AddComponent(m_pRigidBody);
	m_pRigidBody->SetConstraint(RigidBodyConstraintFlag::RotX, true);
	m_pRigidBody->SetConstraint(RigidBodyConstraintFlag::RotZ, true);
	m_pRigidBody->SetConstraint(RigidBodyConstraintFlag::TransY, true);

	std::shared_ptr<physx::PxGeometry>pGeometry(new physx::PxCapsuleGeometry(m_Radius, m_Height*0.5f));
	physx::PxTransform transform = physx::PxTransform::createIdentity();
	transform = physx::PxTransform(physx::PxVec3(0, m_Height*0.5f, 0), physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)));
	pObject->AddComponent(new ColliderComponent(pGeometry, *pMaterial, transform));

}
void CharacterComponent::Update(const GameContext& gameContext)
{
	auto* pInput = gameContext.pInput;

	//Pause?
	if (pInput->IsActionTriggered(eCharacterInput::PAUSE + eCharacterInput::_END * m_PlayerID))
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"PauseScene");
		return;
	}


	const float elapsed = gameContext.pGameTime->GetElapsed();
	m_AnimationAccu -= elapsed;

	//Input
	if (/*m_pControllerComponent && */pInput->IsGamepadConnected(m_PlayerID) && m_pRigidBody)
	{
		if (HandleActions(gameContext, pInput))
		{
			//Rigid
			m_pRigidBody->GetPxRigidBody()->setLinearVelocity(physx::PxVec3(0, 0, 0));

			//Effects
			if (m_pParticleComponent)
			{
				m_pParticleComponent->SetActiveSpawn(false);
			}
			return;
		}
		if (HandleMovement(gameContext, pInput)) return;
	}
	//Not connected
	else if (m_pRigidBody)
	{
		m_pRigidBody->GetPxRigidBody()->setLinearVelocity(physx::PxVec3{ 0,0,0 });
		m_pRigidBody->GetPxRigidBody()->setAngularVelocity(physx::PxVec3{ 0,0,0 });
	}
}
void CharacterComponent::Draw(const GameContext&)
{
}


//Helpers
GameObject* CharacterComponent::RayCast() const
{
	float distance = 5.0f;
	float rad = m_Radius * 1.01f;

	//
	TransformComponent* pTransform = GetTransform();
	const DirectX::XMFLOAT3& forward = pTransform->GetForward();
	const DirectX::XMFLOAT3& world = pTransform->GetWorldPosition();
	DirectX::XMFLOAT3 pos{ world.x + forward.x * rad, world.y + forward.y * rad + m_Height * 0.5f, world.z + forward.z * rad };
	return Raycast::Raycast(pos, forward, distance);
}
bool CharacterComponent::HandleActions(const GameContext& gameContext, InputManager* pInput)
{
	//Target Cupboard
	//***************
	GameObject* pTarget = RayCast();
	BoxComponentBase* pBox = (pTarget) ? (pTarget->GetDynamicComponent<BoxComponentBase>()) : nullptr;
	if (pBox) pBox->Highlight({ 1.25f, 1.25f, 1.25f, 1.0f });

	//Input
	//*****
	UINT actionID{};

	//INTERACT
	actionID = eCharacterInput::INTERACT + eCharacterInput::_END * m_PlayerID;
	if (pInput->IsActionTriggered(actionID))
	{
		//Result:
		//- Item::Use
		//- Box::Interact

		if (m_ItemPointer.IsValid() && m_ItemPointer.Get()->Use(gameContext))
		{
			SetAnimation(eCharacterAnimation::IdleHolding, 1.0f, -1);
			return true;
		}

		//Get Targeted Cupboard
		//*********************
		if (pBox && !m_ItemPointer.IsValid() && m_HolderPointer.IsEmpty() && pBox->Interact(gameContext.pGameTime->GetElapsed()))
		{
			SetAnimation(eCharacterAnimation::Interact, 2.0f, 0.25f);
			return true;
		}
	}


	//PLACE/TAKE
	actionID = eCharacterInput::PLACE + eCharacterInput::_END * m_PlayerID;
	if (pInput->IsActionTriggered(actionID))
	{
		//Get Targeted Cupboard
		//*********************
		if (pBox)
		{
			//Result:
			//- PLACE/TAKE Food
			//- PLACE/TAKE Item

			const bool emptyHolder = m_HolderPointer.IsEmpty();
			const bool emptyItem = !m_ItemPointer.IsValid();

			if (emptyHolder && emptyItem && (pBox->TakeFood(m_HolderPointer) || pBox->TakeItem(m_ItemPointer)))
			{
				SetAnimation(eCharacterAnimation::TakeObject, 5.0f, 0.2f);
				return true;
			}

			if ((!emptyHolder && pBox->AddFood(m_HolderPointer))
				|| (!emptyItem && pBox->AddItem(m_ItemPointer)))
			{
				SetAnimation(eCharacterAnimation::PlaceObject, 5.0f, 0.2f);
				return true;
			}
		}
	}

	//NO VALID INPUT
	return false;
}
bool CharacterComponent::HandleMovement(const GameContext& gameContext, InputManager* pInput)
{
	//MOVEMENT		
	//********
	//********

	//Input
	//*****
	const float elapsed = gameContext.pGameTime->GetElapsed();
	const DirectX::XMFLOAT2 stick = pInput->GetThumbstickPosition(true, m_PlayerID);

	//Calculate Velocity (Lerp Previouse & New)
	//*****************************************
	DirectX::XMFLOAT3 move3DFloat{ stick.x * m_MoveSpeed, 0, stick.y * m_MoveSpeed };
	DirectX::XMVECTOR move3DVector = DirectX::XMLoadFloat3(&move3DFloat);
	DirectX::XMVECTOR velocity3DVector = DirectX::XMLoadFloat3(&m_Velocity);
	velocity3DVector = DirectX::XMVectorLerp(velocity3DVector, move3DVector, elapsed * m_MoveLerp);
	DirectX::XMStoreFloat3(&m_Velocity, velocity3DVector);

	//Apply Velocity
	//**************
	DirectX::XMFLOAT3 velocity = m_Velocity;
	velocity.x *= elapsed;
	velocity.y *= elapsed;
	velocity.z *= elapsed;
	//m_pControllerComponent->Move(velocity);
	m_pRigidBody->GetPxRigidBody()->setLinearVelocity(physx::PxVec3(velocity.x / elapsed, velocity.y / elapsed, velocity.z / elapsed));



	//Animation On Movement
	//*********************
	const float speed = Speed(m_Velocity);
	const bool empty = m_HolderPointer.IsEmpty() && !m_ItemPointer.IsValid();
	const float speedFraction = speed / m_MoveSpeed;
	
	//Walking Or Idle
	if (speedFraction < 0.1f)
	{
		if (empty) SetAnimation(eCharacterAnimation::Idle, 1.0f);
		else SetAnimation(eCharacterAnimation::IdleHolding, 1.0f);
	}
	else
	{
		if (empty) SetAnimation(eCharacterAnimation::Walk, speedFraction);
		else SetAnimation(eCharacterAnimation::WalkHolding, speedFraction);
	}

	//Particles
	if (speedFraction < 0.1f && m_pParticleComponent)
	{
		m_pParticleComponent->SetActiveSpawn(false);
	}
	else if (m_pParticleComponent)
	{
		m_pParticleComponent->SetActiveSpawn(true);
		const DirectX::XMFLOAT4& color = m_pParticleComponent->GetSettings().Color;
		m_pParticleComponent->SetColor({ color.x, color.y, color.z, speedFraction });
		m_pParticleComponent->SetSpawnDelay(float(1 + (1-speedFraction) * 5));
	}

	//ROTATION
	//********
	//********

	//If Input
	if (stick.x != 0.0f || stick.y != 0.0f)
	{
		//Input
		//*****
		auto* pTransform = GetTransform();
		DirectX::XMFLOAT3 targetDirection3DFloat = { -stick.x, 0, stick.y };

		//Axis
		//****
		DirectX::XMVECTOR currentRotation4DVector = DirectX::XMLoadFloat4(&pTransform->GetRotation());
		DirectX::XMVECTOR targetUp3DVector = DirectX::XMLoadFloat3(&pTransform->GetUp());
		DirectX::XMVECTOR targetEye3DVector = DirectX::XMLoadFloat3(&pTransform->GetPosition());
		DirectX::XMVECTOR targetDirection3DVector = DirectX::XMLoadFloat3(&targetDirection3DFloat);

		//Calculate New Rotation (Slerp: Old, Target)
		//*******************************************
		DirectX::XMMATRIX matrix = DirectX::XMMatrixLookToLH(targetEye3DVector, targetDirection3DVector, targetUp3DVector);
		DirectX::XMVECTOR targetRotation4DVector = DirectX::XMQuaternionRotationMatrix(matrix);
		DirectX::XMVECTOR newRotation4DVector = DirectX::XMQuaternionSlerp(currentRotation4DVector, targetRotation4DVector, elapsed * m_RotateSpeed);
		
		//Apply
		//*****
		m_pRigidBody->SetConstraint(RigidBodyConstraintFlag::RotY, false);
		pTransform->Rotate(newRotation4DVector, true);
		m_pRigidBody->SetConstraint(RigidBodyConstraintFlag::RotY, true);
	}

	return true;
}
void CharacterComponent::SetAnimation(eCharacterAnimation animation, float speed, float duration)
{
	auto* pAnimator = (m_pAnimatedModel) ? (m_pAnimatedModel->GetAnimator()) : nullptr;
	if (pAnimator && m_AnimationAccu <= 0.0f)
	{
		m_AnimationAccu = duration;
		if (m_Animation != animation)
		{
			pAnimator->SetAnimation(UINT(abs(animation)));
			m_Animation = animation;
		}
		if (!pAnimator->IsPlaying()) pAnimator->Play();

		pAnimator->SetAnimationSpeed(speed);
		pAnimator->SetPlayReversed(bool(int(animation) < 0));
	}
}
float CharacterComponent::Speed(const DirectX::XMFLOAT3& v) const
{
	return sqrt((v.x*v.x) + (v.y*v.y) + (v.z*v.z));
}
