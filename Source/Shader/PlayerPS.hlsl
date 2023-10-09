#include "FbxModel.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2

SamplerState samplerStates[5] : register(s0);
Texture2D textureMaps[4] : register(t0);

Texture2D maskTexture : register(t15);

float4 main(PSIn psIn) : SV_TARGET
{    
    float4 color = textureMaps[0].Sample(samplerStates[POINT], psIn.texcoord) * psIn.color;
    float alpha = color.a;
    
    float maskValue = maskTexture.Sample(samplerStates[POINT], psIn.texcoord).r;
    
    // ガンマ補正
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);
    
    // ディレクションライト
    float3 directionLight = CalcLightFromDirectionLight(psIn);
    
    // 半球ライト
    float3 hemisphereLight = CalcHemisphereLight(psIn);
    
    float3 finalColor = directionLight + hemisphereLight;
    
    // 二次反射
    finalColor.x += 0.2f;
    finalColor.y += 0.2f;
    finalColor.z += 0.2f;
    
    return float4(max(0, finalColor), alpha) * psIn.color;

}