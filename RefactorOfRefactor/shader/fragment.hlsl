cbuffer Util : register(space3)
{
    float4 DrawColor;
    bool UseTexture;
};

Texture2D Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

float4 main(float2 uv : TEXCOORD) : SV_Target
{
    if (UseTexture)
    {
        return Texture.Sample(Sampler, uv);
    }
    else
    {
        return DrawColor;
    }
}
