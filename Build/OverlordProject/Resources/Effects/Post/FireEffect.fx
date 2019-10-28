//=============================================================================
//// Shader uses position and texture
//=============================================================================

Texture2D gTexture;
//Variables
Texture2D gDiffuseTexture01;
Texture2D gDiffuseTexture02;
Texture2D gMaskTextureEdges;
Texture2D gMaskTextureFull;
float gTime;
float2 gMaskSpeed;
float2 gDiffuseSpeed01;
float2 gDiffuseSpeed02;
float gIntensity;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};


/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState DepthEnabled
{
	DepthEnable = TRUE;
};
/// Create Rasterizer State (Backface culling) 
RasterizerState BackCulling
{
	CullMode = BACK;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};
struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Position = float4(input.Position, 1);
	output.TexCoord = input.TexCoord;
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
	if (gIntensity <= 0) return gTexture.Sample(samPoint, input.TexCoord);

	//Original
	float4 textureSample = gTexture.Sample(samPoint, input.TexCoord);

	//Mask
	float4 maskSample = gMaskTextureEdges.Sample(samPoint, input.TexCoord);
	maskSample *= gMaskTextureFull.Sample(samPoint, input.TexCoord + float2(gMaskSpeed.x - 0.1f, gMaskSpeed.y - 0.1f) * gTime);
	maskSample *= gMaskTextureFull.Sample(samPoint, input.TexCoord - float2(gMaskSpeed.x + 0.1f, gMaskSpeed.y + 0.1f) * gTime);

	//Diffuse
	float4 diffuseSample = gDiffuseTexture01.Sample(samPoint, input.TexCoord + gDiffuseSpeed01 * gTime);
	diffuseSample *= gDiffuseTexture02.Sample(samPoint, input.TexCoord + gDiffuseSpeed02 * gTime);

	//Lerp
	float t = maskSample.x * gIntensity;
	float4 color = (float4)0;
	color.x = textureSample.x * (1 - t) + t * diffuseSample.x;
	color.y = textureSample.y * (1 - t) + t * diffuseSample.y;
	color.z = textureSample.z * (1 - t) + t * diffuseSample.z;
	color.w = textureSample.w * (1 - t) + t * diffuseSample.w;

	return float4(color.xyz, 1.0f);
}


//TECHNIQUE
//---------
technique11 Default
{
    pass P0
    {          
		SetDepthStencilState(DepthEnabled, 0);
		SetRasterizerState(BackCulling);
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

