float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

Texture2D gTexture;
float2 gTextureSize = float2(1, 1);
float2 gScreenSize = float2(10, 10);
float2 gSize = float2(1, 1);
float2 gPivot = float2(0, 0);
float4 gColor = float4(1, 1, 1, 1);

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = CLAMP;
    AddressV = CLAMP;
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
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};
RasterizerState BackCulling
{
    CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	float3 Position : POSITION;
	float2 Scale : TEXCOORD0;
	float Rotation : TEXCOORD1;
};
struct GS_DATA
{
	float4 Position : SV_POSITION;
	float2 TexCoord: TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float2 texCoord, float2x2 uvRotation, float2 screenMul)
{
	GS_DATA object = (GS_DATA)0;

	//Center?
	//object.Position = float4( ((pos.x * screenMul.x) - 0.5f) * 2 + 1, ((pos.y * screenMul.y) - 0.5f) * 2 + 1, 0, 1);
	
	object.Position = float4(((pos.x *screenMul.x) - 0.5f) * 2, ((pos.y * screenMul.y) - 0.5f) * 2, 0, 1);
	texCoord = texCoord * gSize + gPivot;
	texCoord -= float2(0.5f, 0.5f);
	texCoord = mul(texCoord, uvRotation);
	texCoord += float2(0.5f, 0.5f);
	object.TexCoord = texCoord;

	triStream.Append(object);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//Use these variable names
	float3 topLeft, topRight, bottomLeft, bottomRight;
	float2 scale = vertex[0].Scale;
	float2 size = gSize * gTextureSize;
	float2 screenMul = float2(1 / gScreenSize.x, 1 / gScreenSize.y);
	float3 origin = vertex[0].Position;
	origin.y = gScreenSize.y - origin.y; //Solve Axis problem

	topLeft = float3(scale.x * (0.5f * -size.x), scale.y * (0.5f * size.y), 0.0f) + origin;
	topRight = float3(scale.x * (0.5f * size.x), scale.y * (0.5f * size.y), 0.0f) + origin;
	bottomLeft = float3(scale.x * (0.5f * -size.x), scale.y * (0.5f * -size.y), 0.0f) + origin;
	bottomRight = float3(scale.x * (0.5f * size.x), scale.y * (0.5f * -size.y), 0.0f) + origin;

	//This is the 2x2 rotation matrix we need to transform our TextureCoordinates (Texture Rotation)
	float cosrotation = cos(vertex[0].Rotation);
	float sinrotation = sin(vertex[0].Rotation);
	float2x2 uvRotation = { cosrotation, -sinrotation, sinrotation, cosrotation };


	//Create Geometry (Trianglestrip)
	CreateVertex(triStream, bottomLeft, float2(0, 1), uvRotation, screenMul);
	CreateVertex(triStream, topLeft, float2(0, 0), uvRotation, screenMul);
	CreateVertex(triStream, bottomRight, float2(1, 1), uvRotation, screenMul);
	CreateVertex(triStream, topRight, float2(1, 0), uvRotation, screenMul);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET{

	//Simple Texture Sampling
	float4 result = gTexture.Sample(samPoint,input.TexCoord);
	return gColor * result;
}


// Default Technique
technique11 Default
{
	pass p0
	{
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(DisableDepthWriting, 0);

		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
