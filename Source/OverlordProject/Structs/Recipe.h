#pragma once
#include "../Helpers/Enums.h"

struct Recipe final
{
	explicit Recipe() = default;
	~Recipe() = default;

	bool operator==(const Recipe& other) const;
	void Sort();

	static const int SIZE = 4;
	eFoodType foodTypes[SIZE]{ eFoodType::_None };
	eFoodState foodStates[SIZE]{ eFoodState::None };
	eHolderType holderType{ eHolderType::None };

	int Size() const
	{
		int count = 0;
		for (int i = 0; i < SIZE; ++i)
		{
			count += int(ValidType(i));
		}
		return count;
	}
	bool ValidType(int idx) const
	{
		assert(idx < SIZE);
		return foodTypes[idx] != eFoodType::_None;
	}
	bool ValidState(int idx) const
	{
		assert(idx < SIZE);
		return foodStates[idx] != eFoodState::None;
	}
};

