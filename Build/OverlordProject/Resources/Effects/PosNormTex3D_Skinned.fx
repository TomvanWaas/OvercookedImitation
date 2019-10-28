//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.115
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------

float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4x4 gBones[70];

Texture2D gDiffuseMap;
bool gUseDiffuseMap = false;
float4 gColor = { 1,1,1,1 };

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

struct VS_INPUT{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 blendweights : BLENDWEIGHTS;
	float4 blendindices : BLENDINDICES;
};

struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){

	VS_OUTPUT output;

	float4 originalPosition = float4(input.pos, 1);
	float4 transformedPosition = 0;
	float3 transformedNormal = 0;

	//Skinning Magic...
	for (int i = 0; i < 4; ++i)
	{
		float boneindex = input.blendindices[i];
		if (boneindex > -1)
		{
			float blendweight = input.blendweights[i];
			transformedPosition += blendweight * mul(originalPosition, gBones[boneindex]);
			transformedNormal += blendweight * mul(input.normal, (float3x3)gBones[boneindex]);
		}
	}
	transformedPosition.w = 1.0f;

	//Don't forget to change the output.pos & output.normal variables...
	output.pos = mul(transformedPosition, gWorldViewProj);
	output.normal = normalize(mul(transformedNormal, (float3x3)gWorld));

	output.texCoord = input.texCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	float4 diffuseColor = gColor;
	if (gUseDiffuseMap)
	{
		diffuseColor = diffuseColor * gDiffuseMap.Sample( samLinear,input.texCoord );
	}
	float3 color_rgb= diffuseColor.rgb;
	float color_a = diffuseColor.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4( color_rgb , color_a );
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(Solid);
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

