#include "FbxModel.hlsli"

// �萔
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

// EMISSIVE��
Texture2D emissiveTexture : register(t16);

float4 main(PSIn psIn) : SV_TARGET
{
    float mask_value = maskTexture.Sample(samplerStates[0], psIn.texcoord) * psIn.color;

    float4 color = textureMaps[0].Sample(samplerStates[ANISOTROPIC], psIn.texcoord);
    float alpha = color.a;

    // �K���}�␳
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);

    // ������������{�ɍ���Ă݂�
    float3 directionLight = color.rgb * CalcLightFromDirectionLight(psIn);
    
    // �ŏI�I�ȃJ���[
    float3 finalColor = directionLight;    
    
    // HEMISPHERE_LIGHT
#if HEMISPHERE_LIGHT
    float3 hemisphereLight = color.rgb * CalcHemisphereLight(psIn);
    finalColor += hemisphereLight;
#endif

    // �����ł̓��f���̓񎟔��ˌ����^���I�ɏo��
    finalColor.x += 0.2f;
    finalColor.y += 0.2f;
    finalColor.z += 0.2f;
    
    // EMISSIVE
    {
        float3 emissive = emissiveTexture.Sample(samplerStates[LINEAR], psIn.emissiveTexcoord).rgb;
        finalColor += emissive * emissiveIntensity * emissiveColor.rgb;
    }
    
    return float4(max(0, finalColor), alpha) * psIn.color;
}