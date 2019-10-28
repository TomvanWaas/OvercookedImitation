float4x4 gTransform : WorldViewProjection;
Texture2D gSpriteTexture;
float2 gTextureSize;

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
};

DepthStencilState NoDepth
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
	uint TextureId : TEXCOORD0;
	float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
	float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
	float4 Color : COLOR;
	float2 Size : TEXCOORD1; //SizeX, SizeY
};


struct GS_DATA
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
    return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 position, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 rotPivot)
{
    if (rotation != 0)
    {
		//Step 3.
		//Do rotation calculations
		//Transform to origin
		//Rotate
		//Retransform to initial position
		
		//To origin
		position.x = position.x - rotPivot.x;
		position.y = position.y - rotPivot.y;
		
		position = float3((position.x * rotCosSin.x) - (position.y * rotCosSin.y), (position.y * rotCosSin.x) + (position.x * rotCosSin.y), position.z);
		
		position.x = position.x + rotPivot.x;
		position.y = position.y + rotPivot.y;
    }
    //else
    //{
		//Step 2.
		//No rotation calculations (no need to do the rotation calculations if there is no rotation applied > redundant operations)
		//Just apply the pivot offset
    //}

	//Geometry Vertex Output
    GS_DATA geomData = (GS_DATA) 0;
    geomData.Position = mul(float4(position, 1.0f), gTransform);
    geomData.Color = col;
    geomData.TexCoord = texCoord;
    triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//Given Data (Vertex Data)
	float3 position = float3(vertex[0].TransformData.x, vertex[0].TransformData.y, vertex[0].TransformData.z); //Extract the position data from the VS_DATA vertex struct
    float2 offset = float2(position.x, position.y); //Extract the offset data from the VS_DATA vertex struct (initial X and Y position)
    float rotation = vertex[0].TransformData.w; //Extract the rotation data from the VS_DATA vertex struct
	float2 pivot = float2(vertex[0].TransformData2.x, vertex[0].TransformData2.y); //Extract the pivot data from the VS_DATA vertex struct
    float2 scale = float2(vertex[0].TransformData2.z, vertex[0].TransformData2.w); //Extract the scale data from the VS_DATA vertex struct
	float2 size = vertex[0].Size;
	float4 color = vertex[0].Color;
	
	//Vertex Data
	float2 texStart = float2(pivot.x / gTextureSize.x, pivot.y / gTextureSize.y);
	float2 texEnd = float2(texStart.x + size.x / gTextureSize.x, texStart.y + size.y / gTextureSize.y);
	float2 posOffset = float2(size.x * scale.x, size.y * scale.y);

	float2 rotCosSin = float2(0,0);
	if (rotation != 0)
	{
		rotCosSin.x = cos(rotation);
		rotCosSin.y = sin(rotation);
	}
	float2 rotPivot = float2(position.x + posOffset.x * 0.5f, position.y + posOffset.y * 0.5f);


	// LT----------RT //TriangleStrip (LT > RT > LB, LB > RB > RT)
	// |          / |
	// |       /    |
	// |    /       |
	// | /          |
	// LB----------RB
	
	
	
	////VERTEX 1 [LT]
	CreateVertex(triStream, position, color, texStart, rotation, rotCosSin, rotPivot);

	////VERTEX 2 [RT]
	CreateVertex(triStream, float3(position.x + posOffset.x, position.y, position.z), color, float2(texEnd.x, texStart.y), rotation, rotCosSin, rotPivot);

	////VERTEX 3 [LB]
	CreateVertex(triStream, float3(position.x, position.y + posOffset.y, position.z), color, float2(texStart.x, texEnd.y), rotation, rotCosSin, rotPivot);

	////VERTEX 4 [RB]
	CreateVertex(triStream, float3(position.x + posOffset.x, position.y + posOffset.y, position.z), color, texEnd, rotation, rotCosSin, rotPivot);

}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    return gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;
}

// Default Technique
technique11 Default
{
    pass p0
    {
        SetRasterizerState(BackCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(NoDepth,0);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, MainGS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}
