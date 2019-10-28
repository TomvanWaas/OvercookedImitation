#include "stdafx.h"
#include "DiffuseMaterial.h"


DiffuseMaterial::DiffuseMaterial()
	: ColorMaterial(L"./Resources/Effects/PosNormTex3D.fx", L"", {1,1,1,1}, "gColor", "gDiffuseMap")
{
}

