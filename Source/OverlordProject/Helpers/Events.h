#pragma once
#include "Observed.h"

class Events final : public ObservedID
{
public:
	Events(unsigned int id) : ObservedID(id){}

	//Recipe Events
	static const unsigned int EVENT_RECIPE_TIMEOUT = 0;
	static const unsigned int EVENT_RECIPE_TIMELATE = 1;

	//Food Events
	static const unsigned int EVENT_FOOD_STATE_CHANGED = 2;

	//Holder Events
	static const unsigned int EVENT_HOLDER_FOOD_ADDED = 3;
	static const unsigned int EVENT_HOLDER_FOOD_REMOVED = 4;
	static const unsigned int EVENT_HOLDER_FOOD_BURNED = 5;
	static const unsigned int EVENT_HOLDER_FOOD_UNBURNED = 6;

	//Box
	static const unsigned int EVENT_BOX_FIRE_STARTED = 7;
	static const unsigned int EVENT_BOX_FIRE_ENDED = 8;

};