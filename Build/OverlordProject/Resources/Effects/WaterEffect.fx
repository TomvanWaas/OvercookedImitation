/*********************************************************************NVMH3****
*******************************************************************************
$Revision: #4 $

Copyright NVIDIA Corporation 2008
TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
*AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY
LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF
NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

% Simple ocean shader with animated bump map and geometric waves
% Based partly on "Effective Water Simulation From Physical Models", GPU Gems

keywords: material animation environment bumpmap

These paramters have been updated so as to look appropriate when applied to an
FX Composer2 "plane" object


keywords: DirectX10
// Note that this version has twin versions of all techniques,
//   so that this single effect file can be used in *either*
//   DirectX9 or DirectX10

To learn more about shading, shaders, and to bounce ideas off other shader
	authors and users, visit the NVIDIA Shader Library Forums at:

	http://developer.nvidia.com/forums/

*******************************************************************************
******************************************************************************/

//This shader is adopted to the needs of the game
//***********************************************

//GLOBAL MATRICES
//***************
//The World Matrix
float4x4 gWorld : World;
//The World View Projection Matrix
float4x4 gWorldViewProjection : WorldViewProjection;
//The View Inverse Matrix
float4x4 gViewInverse : ViewInverse;

//TWEAKABLE PARAMETERS
//********************
float gTimer = 0;
Texture2D gNormalTexture;
Texture2D gEnvironmentTexture;
float gBumpScale = 1.4f;
float gTextureRepeatX = 8.0f;
float gTextureRepeatY = 4.0f;
float gBumpSpeedX = -0.05f;
float gBumpSpeedY = 0.0f;
float gFresnelBias = 0.1f;
float gFresnelExponent = 4.0f;
float gHDRMultiplier = 3.0f;
float3 gDeepColor = float3(0, 0, 0.1f);
float3 gShallowColor = float3(0, 0.5f, 0.5f);
float3 gReflectionTint = float3(1, 1, 1);
float gReflectionStrength = 1.0f;
float gWaterColorStrength = 1.0f;
float gWaveAmplitude = 0.05f;
float gWaveFrequency = 3.0f;

//SAMPLERS
//********
SamplerState gNormalSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};
SamplerState gEnvironmentSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};


//STRUCTS
//*******
struct VS_Input
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 TexCoord : TEXCOORD0;
};
struct VS_Output
{
	float4 HPosition : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float3 T2WXf1 : TEXCOORD1;	// first row of the 3x3 transform from tangent to cube space
	float3 T2WXf2 : TEXCOORD2;	// second row of the 3x3 transform from tangent to cube space
	float3 T2WXf3 : TEXCOORD3;	// third row of the 3x3 transform from tangent to cube space
	float2 bumpUV0 : TEXCOORD4;
	float2 bumpUV1 : TEXCOORD5;
	float2 bumpUV2 : TEXCOORD6;
	float3 WorldView : TEXCOORD7;
};
struct Wave
{
	float frequency;	// 2*PI / wavelength
	float amplitude;
	float phase;		// speed * 2*PI / wavelength
	float2 direction;
};

//FUNCTIONS
//*********
float EvaluateWave(Wave w, float2 pos, float t)
{
	return w.amplitude * sin(dot(w.direction, pos) * w.frequency + t * w.phase);
}
float EvaluateWaveDerivative(Wave w, float2 pos, float t)
{
	return w.frequency * w.amplitude * cos(dot(w.direction, pos) * w.frequency + t * w.phase);
}
float EvaluateWaveSharp(Wave w, float2 pos, float t, float k)
{
	return w.amplitude * pow(sin(dot(w.direction, pos) * w.frequency + t * w.phase) * 0.5f + 0.5f, k);
}
float EvaluateWaveDerivativeSharp(Wave w, float2 pos, float t, float k)
{
	return k * w.frequency * w.amplitude * pow(sin(dot(w.direction, pos) * w.frequency + t * w.phase)*0.5f + 0.5f, k - 1) * cos(dot(w.direction, pos)*w.frequency + t * w.phase);
}

//SHADER FUNCTIONS
//****************

//VERTEX SHADER
#define NWAVES 2
VS_Output MainVS(VS_Input input)
{
	//Waves
	//*****
	Wave waves[NWAVES] =
	{
		{1.0f, 1.0f, 0.5f, float2(-1,0)},
		{2.0f, 0.5f, 1.3f, float2(-0.7f, -0.7f) }
	};

	//Output
	//******
	VS_Output output = (VS_Output)0;
	waves[0].frequency = gWaveFrequency;
	waves[0].amplitude = gWaveAmplitude;
	waves[1].frequency = gWaveFrequency * 2.0f;
	waves[1].amplitude = gWaveAmplitude * 0.5f;

	float4 po = float4(input.Position, 1.0f);

	//Sum Waves
	//*********
	po.y = 0.0f;
	float ddx = 0.0f;
	float ddy = 0.0f;
	for (int i = 0; i < NWAVES; ++i)
	{
		po.y += EvaluateWave(waves[i], po.xz, gTimer);
		float derivative = EvaluateWaveDerivative(waves[i], po.xz, gTimer);
		ddx += derivative * waves[i].direction.x;
		ddy += derivative * waves[i].direction.y;
	}

	//Compute Tangent Basis
	//*********************
	float3 b = float3(1, ddx, 0);
	float3 t = float3(0, ddy, 1);
	float3 n = float3(-ddx, 1, -ddy);
	output.HPosition = mul(po, gWorldViewProjection);

	//Pass Texture Coordinates for fetching the Normal Map
	//****************************************************
	float2 textureScale = float2(gTextureRepeatX, gTextureRepeatY);
	float2 bumpSpeed = float2(gBumpSpeedX, gBumpSpeedY);
	output.TexCoord = input.TexCoord * textureScale;
	float cycle = fmod(gTimer, 100.0f);
	output.bumpUV0 = input.TexCoord * textureScale + cycle * bumpSpeed;
	output.bumpUV1 = input.TexCoord * textureScale * 2.0f + cycle * bumpSpeed * 4.0f;
	output.bumpUV2 = input.TexCoord * textureScale * 4.0f + cycle * bumpSpeed * 8.0f;

	//Compute the 3x3 Transform from Tangent Space to Object Space
	//************************************************************
	float3x3 objectToTangentSpace;
	objectToTangentSpace[0] = gBumpScale * normalize(t);
	objectToTangentSpace[1] = gBumpScale * normalize(b);
	objectToTangentSpace[2] = normalize(n);

	output.T2WXf1 = mul(objectToTangentSpace, gWorld[0].xyz);
	output.T2WXf2 = mul(objectToTangentSpace, gWorld[1].xyz);
	output.T2WXf3 = mul(objectToTangentSpace, gWorld[2].xyz);

	//Comput the Eye Vector in Cube Space
	//***********************************
	float3 pw = mul(po, gWorld).xyz;
	output.WorldView = gViewInverse[3].xyz - pw;

	//Return Result
	//*************
	return output;
}

//PIXEL SHADER
float4 MainPS(VS_Output input) : SV_TARGET
{
	//Sum Normal Maps
	//***************
	float4 t0 = gNormalTexture.Sample(gNormalSampler, input.bumpUV0) * 2.0f - 1.0f;
	float4 t1 = gNormalTexture.Sample(gNormalSampler, input.bumpUV1) * 2.0f - 1.0f;
	float4 t2 = gNormalTexture.Sample(gNormalSampler, input.bumpUV2) * 2.0f - 1.0f;
	float3 nt = t0.xyz + t1.xyz + t2.xyz;

	//Tangent to World Matrix
	//***********************
	float3x3 m = (float3x3)0;
	m[0] = input.T2WXf1;
	m[1] = input.T2WXf2;
	m[2] = input.T2WXf3;
	float3 nw = mul(m, nt);
	float3 nn = normalize(nw);

	//Reflection
	//**********
	float3 vn = normalize(input.WorldView);
	float3 r = reflect(-vn, nn);
	float4 reflection = gEnvironmentTexture.Sample(gEnvironmentSampler, r);
	reflection.rgb *= (1.0f + reflection.a * gHDRMultiplier);

	float facing = 1.0f - max(dot(vn, nn), 0);
	float fres = gReflectionStrength * (gFresnelBias + (1.0f - gFresnelBias) * pow(facing, gFresnelExponent));

	float3 waterColor = gWaterColorStrength * lerp(gDeepColor, gShallowColor, facing);
	float3 result = waterColor + (fres * reflection.rgb * gReflectionTint);

	//Return Result
	//*************
	return float4(result, 1.0f);
}


//TECHNIQUES
//**********
RasterizerState DisableCulling
{
	CullMode = NONE;
};
DepthStencilState DepthEnabling
{
	DepthEnable = TRUE;
};
DepthStencilState DepthDisabling
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};
BlendState DisableBlend
{
	BlendEnable[0] = FALSE;
};


technique10 Default
{
	pass p0
	{
		SetRasterizerState(DisableCulling);
		SetDepthStencilState(DepthEnabling, 0);
		SetBlendState(DisableBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}




