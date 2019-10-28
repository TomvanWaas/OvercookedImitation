#pragma once
#include "GameObject.h"

class PrototypeObject : public GameObject
{
public:
	explicit PrototypeObject() = default;
	virtual ~PrototypeObject() = default;

	virtual PrototypeObject* Clone(bool addtoScene = false);
protected:
	PrototypeObject* MakeClone();
	void AsClone();
};