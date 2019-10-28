#pragma once
enum class eFoodType
{
	Apple = 0,
	Orange,
	Broccoli,
	Pineapple,
	Zucchini,
	Pepper,
	Pear,
	Banana,
	Lemon,
	
	Mushroom,
	Potato,
	Pumpkin,
	//...



	_Fire,
	_END,
	_None,
	_Any
};
enum class eFoodState
{
	Cooked,
	Baked,
	Base,
	Liquid,
	Cut,
	None,
	Any,
	Burned

	//...
};

enum class eHolderType
{
	None = 0,
	Empty,
	Boiler,
	Pan,
	Plate
	//...
};
enum class eUpdate
{
	None,
	Fire,
	Cut
};



enum class eBoxType
{
	None = 0,
	Base,
	FoodContainer,
	Fire,
	ChoppingBox,
	Bin,
	HolderContainer,
	Delivery
	//...
};

enum class eMealType
{
	Meal01,
	Meal02,
	Meal03,
	Meal04,
	Meal05,


	END,
	None
};

