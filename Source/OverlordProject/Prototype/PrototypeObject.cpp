#include "stdafx.h"
#include "PrototypeObject.h"
#include "PrototypeComponent.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "TransformComponent.h"

PrototypeObject* PrototypeObject::Clone(bool addtoScene)
{
	auto* pClone = MakeClone();

	//Add To Scene
	if (addtoScene)
	{
		SceneManager::GetInstance()->GetActiveScene()->AddChild(pClone);
	}

	pClone->AsClone();
	return pClone;
}

PrototypeObject* PrototypeObject::MakeClone()
{
	auto* pClone = new PrototypeObject();

	//Transform
	const TransformComponent* pTransform = GetTransform();
	TransformComponent* pCloneTransform = pClone->GetTransform();
	pCloneTransform->Translate(pTransform->GetPosition());
	pCloneTransform->Scale(pTransform->GetScale());
	pCloneTransform->Rotate(DirectX::XMLoadFloat4(&pTransform->GetRotation()), true);

	//Components
	auto pComponents = GetDynamicComponents<PrototypeComponent>(false);
	for (size_t i = 0, j = pComponents.size(); i < j; ++i)
	{
		auto* pComponent = pComponents[i];

		auto* pComponentClone = pComponent->Clone();
		pClone->AddComponent(pComponentClone);

		//Observer?
		if (this->IsObservedBy(pComponent)) pClone->AddObserver(pComponentClone);
	}


	//Children
	auto pChildren = GetDynamicChildren<PrototypeObject>();
	for (size_t i = 0, j = pChildren.size(); i < j; ++i)
	{
		auto* pChild = pChildren[i];

		auto* pChildClone = pChild->Clone(false);
		pClone->AddChild(pChildClone);
	}

	return pClone;
}

void PrototypeObject::AsClone()
{
	//Components
	auto pComponents = GetDynamicComponents<PrototypeComponent>(false);
	for (size_t i = 0, j = pComponents.size(); i < j; ++i)
	{
		pComponents[i]->AsClone();
	}

	//children
	auto pChildren = GetDynamicChildren<PrototypeObject>();
	for (size_t i = 0, j = pChildren.size(); i < j; ++i)
	{
		pChildren[i]->AsClone();
	}
}
