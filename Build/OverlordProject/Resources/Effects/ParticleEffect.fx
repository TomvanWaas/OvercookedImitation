float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gWorld : WORLD;
float4x4 gViewInverse : ViewInverse;

float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

Texture2D gTexture;
float2 gTextureSize = float2(1, 1);
float2 gSize = float2(1, 1);
float2 gPivot = float2(0, 0);
float4 gColor = float4(1, 1, 1, 1);
float2 gOffset = float2(0, 0);

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
DepthStencilState EnableDepthWriting
{
	DepthEnable = TRUE;
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
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float2 texCoord, float2x2 uvRotation)
{
	GS_DATA object = (GS_DATA)0;

	object.Position = mul(float4(pos, 1), gWorldViewProj);

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
	float2 offset = gOffset * gTextureSize; 

	//Vertices (Keep in mind that 'origin' contains the center of the quad
	topLeft = float3(scale.x * (0.5f * -size.x + offset.x), scale.y * (0.5f * size.y + offset.y), 0.0f);
	topRight = float3(scale.x * (0.5f * size.x + offset.x), scale.y * (0.5f * size.y + offset.y), 0.0f);
	bottomLeft = float3(scale.x * (0.5f * -size.x + offset.x), scale.y * (0.5f * -size.y + offset.y), 0.0f);
	bottomRight = float3(scale.x * (0.5f * size.x + offset.x), scale.y * (0.5f * -size.y + offset.y), 0.0f);


	//Transform the vertices using the ViewInverse (Rotational Part Only!!! (~ normal transformation)), this will force them to always point towards the camera (cfr. BillBoarding)
	topLeft = mul(topLeft, (float3x3)gViewInverse) + vertex[0].Position;
	topRight = mul(topRight, (float3x3)gViewInverse) + vertex[0].Position;
	bottomLeft = mul(bottomLeft, (float3x3)gViewInverse) + vertex[0].Position;
	bottomRight = mul(bottomRight, (float3x3)gViewInverse) + vertex[0].Position;


	//This is the 2x2 rotation matrix we need to transform our TextureCoordinates (Texture Rotation)
	float cosrotation = cos(vertex[0].Rotation);
	float sinrotation = sin(vertex[0].Rotation);
	float2x2 uvRotation = { cosrotation, -sinrotation, sinrotation, cosrotation };

	//Create Geometry (Trianglestrip)
	CreateVertex(triStream, bottomLeft, float2(0, 1), uvRotation);
	CreateVertex(triStream, topLeft, float2(0, 0), uvRotation);
	CreateVertex(triStream, bottomRight, float2(1, 1), uvRotation);
	CreateVertex(triStream, topRight, float2(1, 0), uvRotation);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET{

	//Simple Texture Sampling
	float4 result = gTexture.Sample(samPoint,input.TexCoord);
	return gColor * result;
}


// Default Technique
technique11 OnWorld
{
    pass p0
    {
        SetRasterizerState(BackCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(EnableDepthWriting,0);

        SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}

technique11 OnScreen
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
