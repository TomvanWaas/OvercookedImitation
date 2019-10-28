#include "stdafx.h"
#include "BoxComponentPassiveUpdator.h"
#include "../Holder/HolderInterfaceComponent.h"


BoxComponentPassiveUpdator::BoxComponentPassiveUpdator(
	eBoxType type, 
	const DirectX::XMFLOAT3& holderOffset,
	const std::vector<eHolderType>& allowedHolders, 
	const ModelInfo& initInfo, 
	eUpdate updateType,
	float updateSpeed)
	: BoxComponentBase(type, holderOffset, allowedHolders, initInfo)
	, m_UpdateType(updateType)
	, m_UpdateSpeed(updateSpeed)
{
}

bool BoxComponentPassiveUpdator::AddFood(HolderPointer& other)
{
	if (m_HolderPointer.IsEmpty() && other.IsValid() && other.Get()->GetType() == eHolderType::Empty)
	{
		return false;
	}
	return BoxComponentBase::AddFood(other);
}

void BoxComponentPassiveUpdator::Update(const GameContext& gameContext)
{
	if (m_HolderPointer.IsValid())
	{
		const float elapsed = gameContext.pGameTime->GetElapsed();
		m_HolderPointer.Get()->Convert(m_UpdateSpeed * elapsed, m_UpdateType, this);
	}
}
