#include "FbxModel.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2

SamplerState samplerStates[5] : register(s0);
Texture2D textureMaps[4] : register(t0);

Texture2D golemEmissive : register(t16);
Texture2D eyeEmissiveTexture : register(t14);

cbuffer EYE_CONSTANTS : register(b7)
{
    float4 eyeEmissiveColor;
    float eyeEmissiveIntensity;
    float3 eyeDummy;
}

float4 main(PSIn psIn) : SV_TARGET
{
    float4 color = textureMaps[0].Sample(samplerStates[POINT], psIn.texcoord) * psIn.color;
    float alpha = color.a;
    
    // ガンマ補正
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);
    
    // ディレクションライト
    float3 directionLight = CalcLightFromDirectionLight(psIn);
    
    // 半球ライト
    float3 hemisphereLight = CalcHemisphereLight(psIn);
    
    //float3 finalColor = directionLight * color.rgb;
    float3 finalColor = directionLight + hemisphereLight * color.rgb;
    
    // 二次反射
    finalColor.x += 0.2f;
    finalColor.y += 0.2f;
    finalColor.z += 0.2f;
    
    float emissive = golemEmissive.Sample(samplerStates[LINEAR], psIn.texcoord).r;
    finalColor += emissive * emissiveIntensity * emissiveColor.rgb;
    
    float eyeEmissive = eyeEmissiveTexture.Sample(samplerStates[LINEAR], psIn.texcoord).r;
    finalColor += eyeEmissive * eyeEmissiveIntensity * eyeEmissiveColor.rgb;
    
    return float4(finalColor, alpha) * psIn.color;
}