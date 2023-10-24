#include "FbxModel.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2

SamplerState samplerStates[4] : register(s0);
Texture2D textureMaps[3] : register(t0);
Texture2D emissiveTexture : register(t16);

float4 main(PSIn psIn) : SV_TARGET
{
    float4 color = textureMaps[0].Sample(samplerStates[LINEAR], psIn.texcoord);
    float alpha = color.a;
    
    float emissive = emissiveTexture.Sample(samplerStates[ANISOTROPIC], psIn.emissiveTexcoord).r;
    float3 finalColor = emissive * emissiveIntensity * emissiveColor.rgb;
	
    return float4(max(0, finalColor), alpha) * psIn.color;
}