float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gWorld : WORLD;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

Texture2D gTexture;
float2 gTextureSize = float2(1, 1);
float2 gSize = float2(1, 1);
float2 gPivot = float2(0, 0);
float4 gColor = float4(1, 1, 1, 1);



SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;

	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0f;
};

DepthStencilState DisableDepthWriting
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

RasterizerState BackCulling
{
    CullMode = BACK;
};
RasterizerState NoCulling
{
	CullMode = NONE;
};

//SHADER STRUCTS
//**************
struct VS_Input
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};
struct VS_Output
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};



//MAIN VERTEX SHADER
//******************
VS_Output MainVS(VS_Input input)
{
	VS_Output output = (VS_Output)0;

	output.Position = mul(float4(input.Position, 1), gWorldViewProj);
	output.Normal = normalize(mul(input.Normal, (float3x3)gWorld));
	output.TexCoord = input.TexCoord * gSize + gPivot;
	return output;
}


//PIXEL SHADER
//************
float4 MainPS(VS_Output input) : SV_TARGET
{
	float4 color = gTexture.Sample(samPoint, input.TexCoord) * gColor;


	return color;
}

// Default Technique
technique11 OneSided
{
    pass p0
    {
        SetRasterizerState(BackCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(DisableDepthWriting,0);

        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}

technique11 DoubleSided
{
	pass p0
	{
		SetRasterizerState(NoCulling);
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(DisableDepthWriting, 0);

		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
