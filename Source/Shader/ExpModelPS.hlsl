#include "FbxModel.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2

SamplerState samplerStates[5] : register(s0);
Texture2D emissiveTextures : register(t16);

float4 main(PSIn psIn) : SV_TARGET
{
    float3 color = { 1, 1, 1 };
	
    float3 emissive = emissiveTextures.Sample(samplerStates[LINEAR], psIn.emissiveTexcoord).rgb;
    color += emissive * emissiveIntensity * emissiveColor.rgb;
    
    return float4(max(0, color.rgb), 1.0f) * psIn.color;
}