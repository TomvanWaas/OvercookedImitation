#include "stdafx.h"
#include "Recipe.h"


bool Recipe::operator==(const Recipe& other) const
{
	bool succes = true;
	for (int i = 0; i < SIZE; ++i)
	{
		succes = succes && (foodTypes[i] == other.foodTypes[i]);
		succes = succes && (foodStates[i] == other.foodStates[i]);
	}
	return succes;
}

void Recipe::Sort()
{
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 1; j < SIZE - i; ++j)
		{
			if (int(foodTypes[j]) < int(foodTypes[j-1]))
			{
				eFoodType bufferType = foodTypes[j];
				foodTypes[j] = foodTypes[j - 1];
				foodTypes[j - 1] = bufferType;
				eFoodState bufferState = foodStates[j];
				foodStates[j] = foodStates[j - 1];
				foodStates[j - 1] = bufferState;
			}
		}
	}
}
