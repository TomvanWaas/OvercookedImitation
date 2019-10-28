#pragma once
#include "../../Helpers/Enums.h"
#include "../../Structs/ModelInfo.h"
#include <map>
struct HolderInfo final
{
	eHolderType type;
	ModelInfo modelInfo;
	//FoodType :: Required State :: Result State
	std::map<eFoodType, std::map<eFoodState, eFoodState>> containments;
	int maxIndex;
	bool canBurn;
	DirectX::XMFLOAT3 foodOffsetWorld;
	DirectX::XMFLOAT3 iconOffsetWorld;
	DirectX::XMFLOAT2 iconOffsetScreen;
	DirectX::XMFLOAT2 stateOffsetScreen;
};