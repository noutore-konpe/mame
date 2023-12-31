#include "FbxModel.hlsli"

// 定数
#define POINT_LIGHT 1
#define SPOT_LIGHT 1
#define LIM_LIGHT 0
#define HEMISPHERE_LIGHT 1

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState samplerStates[5] : register(s0);
Texture2D textureMaps[4] : register(t0);

// SHADOW
SamplerComparisonState comparisonSamplerState : register(s5);
Texture2D shadowMap : register(t8);

// DISSOLVE
Texture2D maskTexture : register(t15);


float4 main(PSIn psIn) : SV_TARGET
{
    float mask_value = maskTexture.Sample(samplerStates[0], psIn.texcoord) * psIn.color;

    float4 color = textureMaps[0].Sample(samplerStates[ANISOTROPIC], psIn.texcoord);
    float alpha = color.a;

#if 1
    // 逆ガンマ補正
    //const float GAMMA = 1.0;
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);
#else
    // ガンマ補正
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, 1.0 / GAMMA);
#endif

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

    // dissolve
#if 0
    alpha = step(parameters.x, mask_value);
#endif

    // SHADOW
#if 1
    const float shadowDepthBias = 0.01;
#else
    // ここでは、サーフェスの法線と光の方向に基づいて、バイアスの最大値が 0.01、最小値が 0.001 になっています。
    const float shadowDepthBias = max(0.01 * (1.0 - dot(N, L)), 0.001);
#endif

    float4 lightViewPosition = mul(psIn.worldPosition, shadowLightViewProjection);
    lightViewPosition = lightViewPosition / lightViewPosition.w;
    float2 lightViewTexcoord = 0;
    lightViewTexcoord.x = lightViewPosition.x * 0.5 + 0.5;
    lightViewTexcoord.y = lightViewPosition.y * -0.5 + 0.5;
    float depth = saturate(lightViewPosition.z - shadowDepthBias);
     
    
    // SHADOW
    float shadowWidth;
    float shadowHight;
    shadowMap.GetDimensions(shadowWidth, shadowHight);

    float shadowWidthOffset = 1 / shadowWidth;
    float shadowHightOffset = 1 / shadowHight;
    
    float3 shadowFactor = 1.0f;
    // shdowFactorひとつだけだと、まわりを取って平均値を出すことできれいに見せる。
    {
        shadowFactor = shadowMap.SampleCmpLevelZero(comparisonSamplerState, lightViewTexcoord, depth).xxx;
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y), depth).xxx; // 右
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y), depth).xxx; // 左
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // 上
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // 下
        
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // 右上
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // 左上
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // 右下
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // 左下
        
        shadowFactor /= 9;
    }
    
    // 最終的なカラー
    float3 finalColor = directionLight;
    
    // POINT_LIGHT
#if POINT_LIGHT
#if POINT_LIGHT_ONE
    float3 pointLight = CalcLightFromPointLight(psIn);
    finalColor += pointLight;
#else
    for (int i = 0; i < 8; ++i)
    {
        float3 pointLight = CalcLightFromPointLight(psIn, pointLig[i]);
        finalColor += pointLight;
    }
#endif// POINT_LIGHT_ONE
#endif
    
    // SPOT_LIGHT
#if SPOT_LIGHT
    float3 spotLight = CalcLightFormSpotLight(psIn);
    finalColor += spotLight;
#endif
    
    // LIM_LIGHT
#if LIM_LIGHT
    float3 limLight = color.rgb * CalcLimLight(psIn);
    finalColor += limLight;
#endif
    
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
    
    return float4(finalColor * shadowFactor, alpha) * psIn.color;
}