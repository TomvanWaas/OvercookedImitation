#pragma once


namespace Raycast
{
	GameObject* Raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& direction, float distance, CollisionGroupFlag ignoreGroups = static_cast<CollisionGroupFlag>(0));
}