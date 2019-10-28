#pragma once
#include "../../Prototype/PrototypeComponent.h"

class HolderBoxNotifier final : public PrototypeComponent
{
public:
	explicit HolderBoxNotifier(const std::vector<unsigned int>& ignoreEvents = {})
		: m_IgnoreEvents(ignoreEvents)
	{}
	virtual ~HolderBoxNotifier() = default;
	virtual PrototypeComponent* Clone() const override;
protected:
	virtual void OnNotify(ObservedID observedID) override;
private:
	std::vector<unsigned int> m_IgnoreEvents;
};

