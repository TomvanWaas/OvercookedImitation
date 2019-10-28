#include "stdafx.h"
#include "Raycast.h"
#include "PhysxProxy.h"
#include "RigidBodyComponent.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "GameObject.h"
#include "DebugRenderer.h"

#define RenderRaycast

GameObject* Raycast::Raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& direction, float distance,
	CollisionGroupFlag ignoreGroups)

{
	//Convert to physx variables
	physx::PxVec3 rayStart{ start.x, start.y, start.z };
	physx::PxVec3 rayDirection{ direction.x, direction.y, direction.z };
	physx::PxReal rayDistance{ distance };
	rayDirection.normalize();

	//Prepare Values
	PhysxProxy* pProxy = SceneManager::GetInstance()->GetActiveScene()->GetPhysxProxy();
	physx::PxQueryFilterData filterData{};
	filterData.data.word0 = ~ignoreGroups;
	physx::PxRaycastBuffer hit{};

	//Raycast
	if (pProxy->Raycast(rayStart, rayDirection, rayDistance, hit, physx::PxHitFlag::eDEFAULT, filterData))
	{
		if (hit.hasBlock)
		{
			auto block = hit.block;
			if (block.actor->userData != nullptr)
			{
				RigidBodyComponent* pRigidComponent = reinterpret_cast<RigidBodyComponent*>(block.actor->userData);

#ifdef _DEBUG
				//RENDER GREEN LINE
				DirectX::XMFLOAT3 end{
					rayStart.x + rayDirection.x * rayDistance,
					rayStart.y + rayDirection.y * rayDistance ,
					rayStart.z + rayDirection.z * rayDistance };
				DebugRenderer::DrawLine(start, end, DirectX::XMFLOAT4{ 0,1,0,1 });
#endif
				return pRigidComponent->GetGameObject();
			}
		}
	}

#ifdef _DEBUG
	//RENDER RED LINE
	DirectX::XMFLOAT3 end{
				rayStart.x + rayDirection.x * rayDistance,
				rayStart.y + rayDirection.y * rayDistance ,
				rayStart.z + rayDirection.z * rayDistance };
	DebugRenderer::DrawLine(start, end, DirectX::XMFLOAT4{ 1,0,0,1 });

#endif
	return nullptr;
}

