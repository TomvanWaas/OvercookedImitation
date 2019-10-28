#pragma once
#include "BaseComponent.h"

class PrototypeComponent abstract : public BaseComponent
{
public:
	PrototypeComponent() = default;
	virtual ~PrototypeComponent() = default;

	PrototypeComponent(const PrototypeComponent& other) = delete;
	PrototypeComponent(PrototypeComponent&& other) noexcept = delete;
	PrototypeComponent& operator=(const PrototypeComponent& other) = delete;
	PrototypeComponent& operator=(PrototypeComponent&& other) noexcept = delete;

	virtual PrototypeComponent* Clone() const = 0;
	virtual void AsClone(){}
};