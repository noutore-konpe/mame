#include "FbxModel.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState samplerStates[3] : register(s0);
Texture2D textureMaps[4] : register(t0);

[earlydepthstencil]
float4 main(PSIn psIn) : SV_TARGET
{
    float4 color = textureMaps[0].Sample(samplerStates[ANISOTROPIC], psIn.texcoord);
    float alpha = color.a;
    
#if 1
    // ガンマ補正
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);
#endif
    
    float3 N = normalize(psIn.worldNormal.xyz);
    // タンジェント、ノーマル
    {
        float3 T = normalize(psIn.worldTangent.xyz);
        float sigma = psIn.worldTangent.w;
        T = normalize(T - N * dot(N, T));
        float3 B = normalize(cross(N, T) * sigma);
        
        float4 normal = textureMaps[1].Sample(samplerStates[LINEAR], psIn.texcoord);
        normal = (normal * 2.0) - 1.0;
        N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
    }
    
#if 0
    float3 L = normalize(directionLig.direction.xyz);
#else
    float3 L = normalize(-lightDirection.xyz);
#endif
    float3 V = normalize(cameraPosition.xyz - psIn.worldPosition.xyz);
    float3 H = normalize(L + V);
    
    float NoL = max(0, dot(N, L));
#if 0
    float NoH = max(0, dot(normalize(psIn.worldNormal.xyz), H));
#else
    float NoH = max(0, dot(N, H));
#endif
    float NoV = max(0, dot(N, V));
    
    float irradiance = smoothstep(0.0, 0.01, NoL);
    
    // ここコンスタントで変更できそう
#if 0
    float3 baseColor = float3(21 / 255.0, 234 / 255.0, 216 / 255.0);
#else
    float3 baseColor = sageColor.rgb;
#endif
    
    float3 ambientColor = baseColor * lerp(0.25, 0, NoV);
    float3 ambient = lerp(0.4 * ambientColor, 0, irradiance);
    
#if 1
    float3 diffuseColor = baseColor * lerp(0.25, 0, NoV);
    float3 diffuse = irradiance * diffuseColor;
#else
    float3 diffuse = CalcLambertDiffuse(directionLig.direction.xyz, directionLig.color.rgb,
        psIn.worldNormal.xyz);
#endif
    
#if 1
    float specularIntensity = pow(NoH, 10);
    //float specularIntensity = pow(NoH, 512);
    float specularIntensitySmooth = smoothstep(0.005, 0.01, specularIntensity);
    float3 specularColor = baseColor;
    float3 specular = specularIntensitySmooth * specularColor;
#else
    float3 specular = CalcPhongSpecular(directionLig.direction.xyz, directionLig.color.rgb,
        psIn.worldPosition.xyz, psIn.worldNormal.xyz);
#endif

    const float rimThreshold = 1.0;
    const float rimAmount = 0.716;
    float rimDot = 1 - NoV;
    float rimIntensity = rimDot * pow(NoL, rimThreshold);
    rimIntensity = smoothstep(rimAmount - 0.1, rimAmount + 0.1, rimIntensity);
    float3 romColor = baseColor;
    float3 rim = rimIntensity * romColor;
    
    const float dh = psIn.localPosition.y / 500.0;
    const float3 foot = 3.0 * float3(0.6, 0.6, 1.0) * exp(-dh * 18.0);
    
    alpha = 0.45; // ここで透明度変更可能
    
    
    return float4(ambient + diffuse + specular + rim + foot, alpha) * psIn.color;
}