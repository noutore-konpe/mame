#include "sprite_dissolve.hlsli"

cbuffer EMISSICE_CONSTANTS : register(b0)
{
    float4 emissiveColor;
    float emissiveIntensity;
};

Texture2D colorMap : register(t0);
SamplerState samplerState : register(s0);

float4 main(VS_OUT psIn) : SV_TARGET
{
    float4 color = colorMap.Sample(samplerState, psIn.texcoord);
    float alpha = color.a;
    
    float3 emissive = colorMap.Sample(samplerState, psIn.texcoord);
    color.rgb += emissive * emissiveIntensity * emissiveColor.rgb;
    
    //return color * psIn.color;
    return float4(max(0, color.rgb), alpha) * psIn.color;
}