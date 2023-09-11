#include "fullscreen_quad.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
#define LINEAR_BORDER_BLACK 3
#define LINEAR_BORDER_WHITE 4
SamplerState samplerStates[5] : register(s0);
Texture2D textureMaps[4] : register(t0);

float4 main(VS_OUT psIn) : SV_TARGET
{
    float4 color = textureMaps[0].Sample(samplerStates[POINT], psIn.texcoord);
    float alpha = color.a;
    
    // FOG
    float3 fogColor = textureMaps[1].Sample(samplerStates[LINEAR_BORDER_BLACK], psIn.texcoord).rgb;
    color.rgb += fogColor;
    
    // Tone Mapping : HDR -> SDR
    const float exposure = 1.2;
    color.rgb = 1 - exp(-color.rgb * exposure);
    
    // Gamma process
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, 1.0 / GAMMA);
    
    return float4(color.rgb, alpha);
}