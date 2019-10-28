#pragma once
#include "ColorMaterial.h"

class DiffuseMaterial final : public ColorMaterial
{
public:
	explicit DiffuseMaterial();
	virtual ~DiffuseMaterial() = default;
private:
};

