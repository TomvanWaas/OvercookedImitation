float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float4x4 gWorldViewProj_Light;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float gShadowMapBias = 0.01f;
float4x4 gBones[70];

Texture2D gDiffuseMap;
Texture2D gShadowMap;
bool gUseDiffuseMap = false;
float4 gColor = { 1,1,1,1 };

SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 BoneIndices : BLENDINDICES;
	float4 BoneWeights : BLENDWEIGHTS;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 lPos : TEXCOORD1;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//complete Vertex Shader 
	//Hint: use the previously made shaders PosNormTex3D_Shadow and PosNormTex3D_Skinned as a guide
	
	float4 originalPosition = float4(input.pos, 1);
	float4 transformedPosition = 0;
	float3 transformedNormal = 0;

	for (int i = 0; i < 4; ++i)
	{
		float boneindex = input.BoneIndices[i];
		if (boneindex > -1)
		{
			float blendweight = input.BoneWeights[i];
			transformedPosition += blendweight * mul(originalPosition, gBones[boneindex]);
			transformedNormal += blendweight * mul(input.normal, (float3x3)gBones[boneindex]);
		}
	}
	transformedPosition.w = 1.0f;

	output.pos = mul(transformedPosition, gWorldViewProj);
	output.normal = normalize(mul(transformedNormal, (float3x3)gWorld));
	output.texCoord = input.texCoord;

	//Project our position to light clip space and store it in lPos
	output.lPos = mul(transformedPosition, gWorldViewProj_Light);


	return output;
}

float2 texOffset(int u, int v)
{
	//return offseted value (our shadow map has the following dimensions: 1280 * 720)
	return float2(u * 1.0f / 1280, v * 1.0f / 720);
}

float EvaluateShadowMap(float4 lpos)
{
	//complete
	//Re-Homogenize
	lpos.xyz /= lpos.w;

	//If outside [-1, 1],[-1, 1],[0, 1] => Not visible to light
	if (lpos.x < -1.0f || lpos.x > 1.0f ||
		lpos.y < -1.0f || lpos.y > 1.0f ||
		lpos.z < 0.0f || lpos.z > 1.0f)
	{
		return 0.0f;
	}

	//Clip space => texture space	[0, 1]
	lpos.x = lpos.x*0.5f + 0.5f;
	lpos.y = lpos.y*(-0.5f) + 0.5f;

	//Apply shadow map bias
	lpos.z -= gShadowMapBias;

	//PCF sampling for shadow map
	float sum = 0.0f;
	float x, y;

	//Perform PCF filtering on a 4x4 texel neighborhood
	for (y = -1.5f; y <= 1.5f; y += 1.0f)
	{
		for (x = -1.5f; x <= 1.5f; x += 1.0f)
		{
			sum += gShadowMap.SampleCmpLevelZero(cmpSampler, lpos.xy + texOffset(x, y), lpos.z);
		}
	}

	//Calc ilumination at fragment
	return sum / 16.0f;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float shadowValue = EvaluateShadowMap(input.lPos);
	float4 diffuseColor = gColor;
	if (gUseDiffuseMap)
	{
		diffuseColor = diffuseColor * gDiffuseMap.Sample( samLinear, input.texCoord );
	}
	
	float3 color_rgb= diffuseColor.rgb;
	float color_a = diffuseColor.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4( color_rgb * shadowValue, color_a );
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

