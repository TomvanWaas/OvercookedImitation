#pragma once
#include "../Templates/SingletonObject.h"
#include "../Helpers/Enums.h"
class PrototypeObject;
class Templates final : public SingletonObject<Templates>
{
public:
	virtual ~Templates();

	bool Add(eFoodType key, PrototypeObject* pTemplate);
	bool Add(eHolderType key, PrototypeObject* pTemplate);

	PrototypeObject* GetClone(eFoodType key) const;
	PrototypeObject* GetClone(eHolderType key) const;

	Templates() = default;
private:

	std::map<eFoodType, PrototypeObject*> m_pFoodTemplates;
	std::map<eHolderType, PrototypeObject*> m_pHolderTemplates;
};


