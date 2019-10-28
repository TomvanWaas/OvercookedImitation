#include "stdafx.h"
#include "HolderBoxNotifier.h"
#include "Observed.h"
#include "GameObject.h"

PrototypeComponent* HolderBoxNotifier::Clone() const
{
	return new HolderBoxNotifier(m_IgnoreEvents);
}

void HolderBoxNotifier::OnNotify(ObservedID observedID)
{
	auto it = std::find(m_IgnoreEvents.begin(), m_IgnoreEvents.end(), observedID.id);
	if (it == m_IgnoreEvents.end())
	{
		Logger::LogInfo(L"HolderBoxNotifier::OnNotify >> Notified Box");
		GameObject* pParent = GetGameObject()->GetParent();
		if (pParent) pParent->Notify(observedID);
	}
}
