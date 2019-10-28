#include "stdafx.h"
#include "BoxComponentDeletor.h"
#include "../../Prototype/PrototypeObject.h"
#include "../../Singletons/Templates.h"
#include "TransformComponent.h"
#include "../Holder/HolderComponent.h"
#include "../Holder/HolderInterfaceComponent.h"
#include "../Food/FoodInterfaceComponent.h"
#include "../../Helpers/Helpers.h"
#include "Destroyer.h"

BoxComponentDeletor::BoxComponentDeletor(
	eBoxType type,
	const DirectX::XMFLOAT3& holderOffset,
	const std::vector<eHolderType>& allowedHolders,
	const ModelInfo& initInfo,
	const std::map<eHolderType, 
	eDeleteSpecifier>& deleteInfo,
	const DirectX::XMFLOAT3& startOffset,
	const DirectX::XMFLOAT3& endOffset,
	std::function<void(const HolderPointer&)> onRemove)

	: BoxComponentBase(type, holderOffset, allowedHolders, initInfo)
	, m_DeleteInfo(deleteInfo)
	, m_StartOffset(startOffset)
	, m_EndOffset(endOffset)
	, m_OnRemove(onRemove)
	, m_pObjectsToDelete()
{
}

bool BoxComponentDeletor::AddFood(HolderPointer& other)
{
	if (other.IsValid() && other.Get()->GetHolderComponent())
	{
		auto type = other.Get()->GetType();
		auto it = m_DeleteInfo.find(type);
		if (it != m_DeleteInfo.end())
		{
			//Execute 'OnRemove' Function
			//***************************
			if (m_OnRemove) m_OnRemove(other);

			//Remove Depending on specifier
			//*****************************
			auto specifier = it->second;
			switch (specifier)
			{
			case eDeleteSpecifier::Nothing:
				break;
			case eDeleteSpecifier::Food:
			{
				//Delete Food
				//***********
				auto* pHolder = other.Get()->GetHolderComponent();
				while (pHolder->GetIndex() > 0)
				{
					auto* pFood = pHolder->RemoveFood();
					if (pFood) AddToDelete(pFood->GetGameObject());
				}
			}
			break;
			case eDeleteSpecifier::All:
			{
				//Remove & AddToDelete holder, Replace with new Empty
				//***************************************************
				HolderInterfaceComponent* pPrevious = other.Remove();
				if (pPrevious) AddToDelete(pPrevious->GetGameObject());
				Helpers::AddHolder(other, eHolderType::Empty);
			}
			break;
			}

			AddSound();
			return true;
		}
	}
	return false;
}
bool BoxComponentDeletor::TakeFood(HolderPointer&)
{
	return false;
}


void BoxComponentDeletor::Update(const GameContext& gameContext)
{
	const float speed = 1.5f;
	const float elapsed = gameContext.pGameTime->GetElapsed();

	//Objects Move
	//************
	const float offset = elapsed * speed;
	for (auto& pair : m_pObjectsToDelete)
	{
		pair.second += offset;
		auto* pTransform = pair.first->GetTransform();
		DirectX::XMFLOAT3 pos = Lerp(m_StartOffset, m_EndOffset, pair.second);
		pTransform->Translate(pos);	
	}

	//Delete EndOffset
	//****************
	auto it = std::remove_if(m_pObjectsToDelete.begin(), 
		m_pObjectsToDelete.end(), 
		[this, offset](const std::pair<GameObject*, float>& pair)
	{
		const auto& pos = pair.first->GetTransform()->GetPosition();
		return IsOnEnd(pos, offset);

	});
	if (it != m_pObjectsToDelete.end())
	{
		auto *pObject = GetGameObject();
		std::for_each(it, m_pObjectsToDelete.end(),
			[pObject](std::pair<GameObject*, float>& pair)
		{
			pObject->RemoveChild(pair.first);
			Destroyer::GetInstance().Destroy(pair.first);
		});
		m_pObjectsToDelete.erase(it, m_pObjectsToDelete.end());
	}

}
void BoxComponentDeletor::AddToDelete(GameObject* pObject)
{
	if (!pObject) return;
	m_pObjectsToDelete.push_back({ pObject, 0.0f });
	pObject->GetTransform()->Translate(m_StartOffset);
	GetGameObject()->AddChild(pObject);
}


DirectX::XMFLOAT3 BoxComponentDeletor::Lerp(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b, float t) const
{
	return { 
		a.x * (1 - t) + b.x * t, 
		a.y * (1 - t) + b.y * t,
		a.z * (1 - t) + b.z * t 
	};
}
bool BoxComponentDeletor::IsOnEnd(const DirectX::XMFLOAT3& p, float) const
{
	DirectX::XMFLOAT3 end = { m_EndOffset.x - m_StartOffset.x, m_EndOffset.y - m_StartOffset.y, m_EndOffset.z - m_StartOffset.z };
	DirectX::XMFLOAT3 cur = { p.x - m_StartOffset.x, p.y - m_StartOffset.y, p.z - m_StartOffset.z };
	float endsqd = { end.x*end.x + end.y*end.y + end.z*end.z };
	float cursqd = { cur.x*cur.x + cur.y*cur.y + cur.z*cur.z };
	return cursqd > endsqd;
}

