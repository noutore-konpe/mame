#include "FbxModel.hlsli"

// 定数
#define HEMISPHERE_LIGHT 1

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState samplerStates[5] : register(s0);
Texture2D textureMaps[4] : register(t0);

// SHADOW
SamplerComparisonState comparisonSamplerState : register(s5);
Texture2D shadowMap : register(t8);


float4 main(PSIn psIn) : SV_TARGET
{
    float4 color = textureMaps[0].Sample(samplerStates[ANISOTROPIC], psIn.texcoord);
    float alpha = color.a;

    // ガンマ補正
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);

#if 0
    float3 N = normalize(psIn.worldNormal.xyz);

#if 1
    float3 T = normalize(psIn.worldTangent.xyz);
    float sigma = psIn.worldTangent.w;
    T = normalize(T - N * dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);

    float4 normal = textureMaps[1].Sample(samplerStates[LINEAR], psIn.texcoord);
    normal = (normal * 2.0) - 1.0;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
#endif
    
    float3 L = normalize(-lightDirection.xyz);
    float3 diffuse = color.rgb * max(0, dot(N, L));
#endif
#if 0
    // todo : 256の部分をconstantで操作したい
    float3 V = normalize(cameraPosition.xyz - psIn.worldPosition.xyz);
    float3 specular = pow(max(0, dot(N, normalize(V + L))), 256);
    // SPECULAR
    specular *= 0.1;
#else
    // 魔導書をお手本に作ってみた
    float3 directionLight = color.rgb * CalcLightFromDirectionLight(psIn);
#endif



    
    // 最終的なカラー
    float3 finalColor = directionLight;
    

    
    // HEMISPHERE_LIGHT
#if HEMISPHERE_LIGHT
    float3 hemisphereLight = color.rgb * CalcHemisphereLight(psIn);
    finalColor += hemisphereLight;
#endif

    // ここではモデルの二次反射光を疑似的に出す
    finalColor.x += 0.2f;
    finalColor.y += 0.2f;
    finalColor.z += 0.2f;
    
    // EMISSIVE
    {
        float3 emissive = textureMaps[2].Sample(samplerStates[LINEAR], psIn.texcoord).rgb;
        const float emissiveIntensity = 3.0f;
        finalColor += emissive * emissiveIntensity;
    }
    
    return float4(finalColor, alpha) * psIn.color;
}