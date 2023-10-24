#include "FbxModel.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2

SamplerState samplerStates[5] : register(s0);
Texture2D textureMaps[4] : register(t0);

Texture2D golemEmissive[2] : register(t16);
Texture2D golemNormal : register(t14);

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
    
    float3 N = normalize(psIn.worldNormal.xyz);
    float3 T = normalize(psIn.worldTangent.xyz);
    float sigma = psIn.worldTangent.w;
    T = normalize(T - N * dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);

    float4 normal = golemNormal.Sample(samplerStates[LINEAR], psIn.texcoord);
    normal = (normal * 2.0) - 1.0;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
    
    float3 L = normalize(-lightDirection.xyz);
    float3 diffuse = color.rgb * max(0, dot(N, L));
    float3 V = normalize(cameraPosition.xyz - psIn.worldPosition.xyz);
    float3 specular = pow(max(0, dot(N, normalize(V + L))), 128);
    
    float3 directionLight = diffuse + specular;
    
    // ディレクションライト
    //float3 directionLight = CalcLightFromDirectionLight(psIn);
    
    // 半球ライト
    float3 hemisphereLight = CalcHemisphereLight(psIn);
    
    //float3 finalColor = directionLight * color.rgb;
    float3 finalColor = directionLight + hemisphereLight * color.rgb;
    
    // 二次反射
    finalColor.x += 0.2f;
    finalColor.y += 0.2f;
    finalColor.z += 0.2f;
    
    float emissive = golemEmissive[0].Sample(samplerStates[LINEAR], psIn.texcoord).r;
    finalColor += emissive * emissiveIntensity * emissiveColor.rgb;
    
    float eyeEmissive = golemEmissive[1].Sample(samplerStates[LINEAR], psIn.texcoord).r;
    finalColor += eyeEmissive * eyeEmissiveIntensity * eyeEmissiveColor.rgb;
    
    return float4(finalColor, alpha) * psIn.color;
}