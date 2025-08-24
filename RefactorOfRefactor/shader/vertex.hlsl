cbuffer ScreenSize : register(space1)
{
    uint ScreenWidth;
    uint ScreenHeight;
}

struct VertexIn
{
    uint id : SV_VertexID;
    float2 position : POSITION;
};

struct VertexOut
{
    float4 position : SV_Position;
    float2 uv : TEXTCOORD;
};

VertexOut main(VertexIn input)
{
    float2 normalizedPosition =
    {
        input.position.x / (ScreenWidth / 2.0f) - 1.0f,
        -(input.position.y / (ScreenHeight / 2.0f) - 1.0f)
    };
    
    float2 uvLookupTable[4] =
    {
        float2(0, 0),
        float2(1, 0),
        float2(0, 1),
        float2(1, 1)
    };
    
    VertexOut output;
    output.position = float4(normalizedPosition, 0, 1);
    output.uv = uvLookupTable[input.id];
    return output;
}
