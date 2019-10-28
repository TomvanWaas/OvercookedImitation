#include "stdafx.h"
#include "TopDownCameraComponent.h"
#include "TransformComponent.h"


TopDownCameraComponent::TopDownCameraComponent(
	const MinMax<float>& offsetLimits, 
	float angleSpeed, float offsetSpeed)
	: m_OffsetLimits(offsetLimits)
	, m_AngleSpeed(angleSpeed)
	, m_OffsetSpeed(offsetSpeed)
	, m_Accu(0.0f)
	, m_Pivot()
{}

void TopDownCameraComponent::Initialize(const GameContext&)
{
	m_Pivot = GetTransform()->GetPosition();
}
void TopDownCameraComponent::Update(const GameContext& gameContext)
{
	const float elapsed = gameContext.pGameTime->GetElapsed();
	m_Accu += elapsed;

	//Angle
	//*****
	float angleAccu = m_Accu * m_AngleSpeed;
	angleAccu = ModF(angleAccu, 1.0f, 10000000);
	float angle = 360.0f * angleAccu;


	//Offset
	//******
	const int precision = 100;
	const float constant = 2.0f;

	float offsetAccu = m_Accu * m_OffsetSpeed;
	offsetAccu = ModF(offsetAccu, constant, precision);
	if (offsetAccu > 1.0f) offsetAccu = constant - offsetAccu;

	//Lerp
	float offset = m_OffsetLimits.min * (1 - offsetAccu) + m_OffsetLimits.max * offsetAccu;

	//Set Offset (Rotate Right around Forward by Angle and multiply by Offset)
	//************************************************************************
	TransformComponent* pTransform = GetTransform();
	DirectX::XMVECTOR forwardVector = DirectX::XMLoadFloat3(&pTransform->GetForward());
	DirectX::XMVECTOR rightVector = DirectX::XMLoadFloat3(&pTransform->GetRight());
	rightVector = DirectX::XMVector3Rotate(rightVector, DirectX::XMQuaternionRotationAxis(forwardVector, DirectX::XMConvertToRadians(angle)));
	DirectX::XMFLOAT3 offsetFloat3{};
	DirectX::XMStoreFloat3(&offsetFloat3, rightVector);

	//Apply
	pTransform->Translate(m_Pivot.x + offsetFloat3.x * offset, m_Pivot.y + offsetFloat3.y * offset, m_Pivot.z + offsetFloat3.z * offset);
}
void TopDownCameraComponent::Draw(const GameContext&)
{
}
float TopDownCameraComponent::ModF(float f, float m, int precision) const
{
	int fint = int(f * precision);
	int mint = int(m * precision);
	return float(fint % mint) / precision;
}
