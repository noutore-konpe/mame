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
    
    // �K���}�␳
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);
    
    // �f�B���N�V�������C�g
#if 1
    float3 directionLight = CalcLightFromDirectionLight(psIn);
#else
    float3 N = normalize(psIn.worldNormal.xyz);
    float3 T = normalize(psIn.worldTangent.xyz);
    float sigma = psIn.worldTangent.w;
    T = normalize(T - N * dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);

    float4 normal = textureMaps[1].Sample(samplerStates[LINEAR], psIn.texcoord);
    normal = (normal * 2.0) - 1.0;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
    
    float3 L = normalize(-lightDirection.xyz);
    float3 diffuse = color.rgb * max(0, dot(N, L));
    float3 V = normalize(cameraPosition.xyz - psIn.worldPosition.xyz);
    float3 specular = pow(max(0, dot(N, normalize(V + L))), 128);
    
    //float3 directionLight = diffuse + specular;
    float3 directionLight = diffuse;
#endif
    
    // �������C�g
    float3 hemisphereLight = CalcHemisphereLight(psIn);
    
    float3 finalColor = directionLight + hemisphereLight * color.rgb;
    
    // �񎟔���
    finalColor.x += 0.2f;
    finalColor.y += 0.2f;
    finalColor.z += 0.2f;
    
    return float4(max(0, finalColor), alpha) * psIn.color;

}