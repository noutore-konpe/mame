#include "AuraEffect.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState samplerStates[3] : register(s0);
Texture2D textureMap[4] : register(t0);

float4 main(GS_OUT psIn) : SV_TARGET
{
    float4 color = textureMap[0].Sample(samplerStates[LINEAR], psIn.texcoord);
    
    return color;
}