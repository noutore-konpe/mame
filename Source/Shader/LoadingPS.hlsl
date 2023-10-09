SamplerState samplerState : register(s0);
Texture2D textureMap : register(t8);

float4 main(float4 position :SV_POSITION,float2 texcoord : TEXCOORD) : SV_TARGET
{
    float4 color = textureMap.Sample(samplerState, texcoord);
    
    return color;
}