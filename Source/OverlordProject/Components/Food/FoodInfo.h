#pragma once
#include "../../Helpers/Enums.h"
#include "../../Structs/ModelInfo.h"

struct FoodConversion final
{
	float time;
	eUpdate update;
	eFoodState state;
};
struct FoodInfo final
{
	eFoodType type;

	//Required State :: Duration, Update & Result State
	std::map<eFoodState, std::vector<FoodConversion>> conversions;
	std::map<eFoodState, ModelInfo> models;
};