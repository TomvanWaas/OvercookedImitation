#include "stdafx.h"
#include "BoxComponentActiveUpdator.h"
#include "../Holder/HolderInterfaceComponent.h"
#include "../Food/FoodComponent.h"
#include "TransformComponent.h"
#include "SoundManager.h"
#include "../../Helpers/SoundID.h"

BoxComponentActiveUpdator::BoxComponentActiveUpdator(
	eBoxType type,
	const DirectX::XMFLOAT3& holderOffset,
	const std::vector<eHolderType>& allowedHolders, 
	const ModelInfo& initInfo, 
	eUpdate updateType, 
	float updateSpeed,
	float updateDelay,
	const std::function<void(const HolderPointer&)>& onUpdate,
	const std::function<void(const HolderPointer&)>& onFinished)
	: BoxComponentBase(type, holderOffset, allowedHolders, initInfo)
	, m_UpdateType(updateType)
	, m_UpdateSpeed(updateSpeed)
	, m_UpdateDelay(updateDelay)
	, m_Accu(0.0f)
	, m_OnUpdate(onUpdate)
	, m_OnFinished(onFinished)
	, m_Finished(true)
	, m_InteractSound(eSoundID::ChopSingle)
{
}

bool BoxComponentActiveUpdator::AddFood(HolderPointer& other)
{
	if (BoxComponentBase::AddFood(other))
	{
		m_Accu = 0.0f;
		m_Finished = false;
		return true;
	}
	return false;
}

bool BoxComponentActiveUpdator::TakeFood(HolderPointer& other)
{
	if (BoxComponentBase::TakeFood(other))
	{
		m_Accu = 0.0f;
		m_Finished = false;
		return true;
	}
	return false;
}

bool BoxComponentActiveUpdator::Interact(float elapsed)
{
	if (m_HolderPointer.IsValid() && !m_HolderPointer.IsEmpty())
	{
		m_Accu += elapsed * m_UpdateSpeed;
		if (m_Accu >= m_UpdateDelay)
		{
			m_Accu -= m_UpdateDelay;
			eConvertReturn r = m_HolderPointer.Get()->Convert(m_UpdateDelay, m_UpdateType, this);

			switch ( r)
			{
			case eConvertReturn::Finished:
			{
				if (!m_Finished) InteractSound();
				m_Finished = true;
				if (m_OnFinished) m_OnFinished(m_HolderPointer);
				return false;
			}
			break;
			case eConvertReturn::Converting:
			{
				m_OnUpdate(m_HolderPointer);
				InteractSound();
				return true;
			}
			break;
			}
		}
		return !m_Finished;
	}
	return false;
}

void BoxComponentActiveUpdator::InteractSound()
{
	SoundManager::GetInstance()->PlaySoundByID(int(m_InteractSound), int(eChannelID::Effect01), 0.025f, eSoundMerge::Overlap);
}
