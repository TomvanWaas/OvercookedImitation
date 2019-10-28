#pragma once
#include "../Holder/HolderPointer.h"
#include "../../Components/Item/ItemPointer.h"

class RigidBodyComponent;
class ControllerComponent;
class ModelComponent;
class ParticleEmitterComponent;
class CharacterComponent final : public BaseComponent
{
public:
	enum eCharacterInput : UINT
	{
		INTERACT = 0,
		PLACE = 1,
		PAUSE = 2,
		_END = 3
	};
	enum eCharacterAnimation : int
	{
		Idle = 0,
		IdleHolding = 1,
		Walk = 2,
		WalkHolding = 3,
		Interact = 4,
		TakeObject = 5,
		PlaceObject = -6, //- => Reversed


		None
	};
	explicit CharacterComponent(GamepadIndex playerID,
		const DirectX::XMFLOAT4 color,
		float radius = 2, float height = 5, float moveSpeed = 100, float rotateSpeed = 100, 
		const DirectX::XMFLOAT3& holderOffset = { 0,2,0 });
	virtual ~CharacterComponent() = default;
	CharacterComponent(const CharacterComponent& other) = delete;
	CharacterComponent(CharacterComponent&& other) noexcept = delete;
	CharacterComponent& operator=(const CharacterComponent& other) = delete;
	CharacterComponent& operator=(CharacterComponent&& other) noexcept = delete;

protected:
	virtual void PostInitialize(const GameContext& gameContext) override;
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:
	HolderPointer m_HolderPointer;
	ItemPointer m_ItemPointer;
	DirectX::XMFLOAT4 m_Color;
	DirectX::XMFLOAT3 m_Velocity;
	float m_Radius;
	DirectX::XMFLOAT3 m_ObjectOffset;
	float m_Height;
	float m_MoveSpeed;
	float m_MoveLerp;
	float m_RotateSpeed;
	float m_AnimationAccu = 0.0f;
	GamepadIndex m_PlayerID;
	eCharacterAnimation m_Animation;

	ModelComponent* m_pAnimatedModel = nullptr;
	RigidBodyComponent* m_pRigidBody = nullptr;
	ParticleEmitterComponent* m_pParticleComponent = nullptr;
	//ControllerComponent* m_pControllerComponent = nullptr;


	//Helper
	GameObject* RayCast() const;
	bool HandleActions(const GameContext& gameContext, InputManager* pInput);
	bool HandleMovement(const GameContext& gameContext, InputManager* pInput);
	void SetAnimation(eCharacterAnimation animation, float speed = 1.0f, float duration = -1.0f);
	float Speed(const DirectX::XMFLOAT3& v) const;
};

