#include "FbxModel.hlsli"

[earlydepthstencil]
float4 main(PSIn psIn) : SV_TARGET
{
    float3 N = normalize(psIn.worldNormal.xyz);
    float3 L = normalize(-lightDirection.xyz);
    float3 V = normalize(cameraPosition.xyz - psIn.worldPosition.xyz);
    float3 H = normalize(L + V);    
    float NoL = max(0, dot(N, L));
    float NoH = max(0, dot(N, H));
    float NoV = max(0, dot(N, V));    
    float irradiance = smoothstep(0.0, 0.01, NoL);
    float3 baseColor = float3(0.0f, 0.0f, 1.0f);
    
    float3 ambientColor = baseColor * lerp(0.25, 0, NoV);
    float3 ambient = lerp(0.4 * ambientColor, 0, irradiance);
    
    float3 diffuseColor = baseColor * lerp(0.25, 0, NoV);
    float3 diffuse = irradiance * diffuseColor;
    
    float specularIntensity = pow(NoH, 10);
    float specularIntensitySmooth = smoothstep(0.005, 0.01, specularIntensity);
    float3 specularColor = baseColor;
    float3 specular = specularIntensitySmooth * specularColor;
    
    const float rimThreshold = 1.0;
    const float rimAmount = 0.716;
    float rimDot = 1 - NoV;
    float rimIntensity = rimDot * pow(NoL, rimThreshold);
    rimIntensity = smoothstep(rimAmount - 0.1, rimAmount + 0.1, rimIntensity);
    float3 romColor = baseColor;
    float3 rim = rimIntensity * romColor;
    
    const float dh = psIn.localPosition.y / 500.0;
    const float3 foot = 3.0 * float3(1.0, 0.0, 0.0) * exp(-dh * 18.0);
    
    float alpha = 0.45; // Ç±Ç±Ç≈ìßñæìxïœçXâ¬î\
    
    
    return float4(ambient + diffuse + specular + rim + foot, alpha) * psIn.color;
}