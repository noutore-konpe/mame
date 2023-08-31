#include "skinned_mesh.hlsli"

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

/////////////////////////
//      �֐��錾        //
/////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);

float4 main(VS_OUT pin) : SV_TARGET
{
    float mask_value = maskTexture.Sample(samplerStates[0],pin.texcoord) * pin.color;

    float4 color = textureMaps[0].Sample(samplerStates[ANISOTROPIC],pin.texcoord);
    float alpha = color.a;

#if 1
    // �t�K���}�␳
    //const float GAMMA = 1.0;
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);
#else
    // �K���}�␳
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, 1.0 / GAMMA);
#endif

    float3 N = normalize(pin.worldNormal.xyz);

#if 1
    float3 T = normalize(pin.worldTangent.xyz);
    float sigma = pin.worldTangent.w;
    T = normalize(T - N * dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);

    float4 normal = textureMaps[1].Sample(samplerStates[LINEAR], pin.texcoord);
    normal = (normal * 2.0) - 1.0;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
#endif
    
    float3 L = normalize(-lightDirection.xyz);
    float3 diffuse = color.rgb * max(0, dot(N, L));
#if 1
    // todo : 256�̕�����constant�ő��삵����
    float3 V = normalize(cameraPosition.xyz - pin.worldPosition.xyz);
    float3 specular = pow(max(0, dot(N, normalize(V + L))), 256);
    // SPECULAR
    specular *= 0.1;
#else
    // ������������{�ɍ���Ă݂�
    float3 refVec = reflect(L, N);
    float3 toEye = cameraPosition - pin.worldPosition;
    toEye = normalize(toEye);
    float3 specular = color.rgb * pow(max(0, dot(refVec, toEye)), 5.0f);
#endif

    // dissolve
#if 0
    alpha = step(parameters.x, mask_value);
#endif

    // SHADOW
#if 1
    const float shadowDepthBias = 0.01;
#else
    // �����ł́A�T�[�t�F�X�̖@���ƌ��̕����Ɋ�Â��āA�o�C�A�X�̍ő�l�� 0.01�A�ŏ��l�� 0.001 �ɂȂ��Ă��܂��B
    const float shadowDepthBias = max(0.01 * (1.0 - dot(N, L)), 0.001);
#endif

    float4 lightViewPosition = mul(pin.worldPosition, lightViewProjection);
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
    // shdowFactor�ЂƂ������ƁA�܂�������ĕ��ϒl���o�����Ƃł��ꂢ�Ɍ�����B
    {
        shadowFactor = shadowMap.SampleCmpLevelZero(comparisonSamplerState, lightViewTexcoord, depth).xxx;
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y), depth).xxx; // �E
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y), depth).xxx; // ��
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // ��
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // ��
        
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // �E��
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // ����
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // �E��
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // ����
        
        shadowFactor /= 9;
    }
    

    // POINT_LIGHT
    float3 light; // �ŏI�I�Ȍ��ʂ�����
    {
        // �T�[�t�F�C�X�ɓ��˂���|�C���g���C�g�̌��̌������v�Z����
        float3 lightDirection = pin.worldPosition.xyz - ptPosition;
        
        // ���K������
        lightDirection = normalize(lightDirection);

        // �����Ȃ���Lambert�g�U���ˌ����v�Z����
        float3 diffusePoint = CalcLambertDiffuse(
            lightDirection,     // ���C�g�̕���
            ptColor,            // ���C�g�̐F
            pin.worldNormal.xyz // �T�[�t�F�C�X�̖@��
        );
        
        // �����Ȃ���Phong���ʔ��ˌ����v�Z����
        float3 specularPoint = CalcPhongSpecular(
            lightDirection,         // ���C�g�̕���
            ptColor,                // ���C�g�̃J���[
            pin.worldPosition.xyz,  // �T�[�t�F�C�X�̃��[���h���W
            pin.worldNormal.xyz     // �T�[�t�F�C�X�̖@��
        );

        // �|�C���g���C�g�Ƃ̋������v�Z����
        float distance = length(pin.worldPosition.xyz - ptPosition);
        
        // �e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
        float affect = 1.0f - 1.0f / ptRange * distance;
        
        // �e���͂��}�C�i�X�ɂȂ�Ȃ��悤�ɏC����������
        affect = max(0.0f, affect);
        
        // �e�����w���֐��I�ɂ���B�����3�悵�Ă�
        affect = pow(affect, 3.0f);
        
        // �g�U���ˌ��Ƌ��ʔ��ˌ��ɉe��������Z���ĉe������߂�
        diffusePoint *= affect;
        specularPoint *= affect;
        
        // 2�̔��ˌ������Z���čŏI�I�Ȕ��ˌ������߂�
        float3 diffuseLight = diffusePoint + diffuse;
        float3 specularLight = specularPoint + specular;
        
        light = diffuseLight + specularLight;
    }
    

    // �����ł̓��f���̓񎟔��ˌ����^���I�ɏo��
#if 1
    // float3 finalColor = diffuse + specular;
    float3 finalColor = light;   // �|�C���g���C�g�l��
#else
    float3 finalColor = diffuse;
#endif
    finalColor.x += 0.2f;
    finalColor.y += 0.2f;
    finalColor.z += 0.2f;

    return float4(finalColor * shadowFactor, alpha) * pin.color;

    //return float4((diffuse + specular) * shadowFactor/*SHADOW*/, alpha) * pin.color;

    //color.a *= alpha;
    //return color;
    

    //return color * pin.color;
    //return float4(diffuse, alpha) * pin.color;
    
    //return float4(diffuse + specular, alpha) * pin.color;
}


// Lambert�g�U���ˌ����v�Z����
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    // �s�N�Z���̖@���ƃ��C�g�̕����̓��ς��v�Z����
    float t = dot(normal, lightDirection) * -1.0f;
    
    // ���ς̒l��0�ȏ�̒l�ɂ���
    t = max(0.0f, t);
    
    // �g�U���ˌ����v�Z����
    return lightColor * t;
}

// Phong���ʔ��ˌ����v�Z����
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
    // ���˃x�N�g�������߂�
    float3 refVec = reflect(lightDirection, normal);
    
    // �������������T�[�t�F�C�X���王�_�ɐL�т�x�N�g�������߂�
    float3 toEye = cameraPosition.xyz - worldPos;
    toEye = normalize(toEye);
    
    // ���ʔ��˂̋��������߂�
    float t = dot(refVec, toEye);
    
    // ���ʔ��˂̋�����0�ȏ�ɂ���
    t = max(0.0f, t);
    
    // ���ʔ��˂̋������i��
    t = pow(t, 5.0f);
    
    // ���ʔ��ˌ������߂�
    return lightColor * t;
}

#if 0
#include "skinned_mesh.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3]:register(s0);
Texture2D texture_maps[4]:register(t0);
Texture2D mask_texture : register(t15);
float4 main(VS_OUT pin) :SV_TARGET
{
    float mask_value = mask_texture.Sample(sampler_states[0],pin.texcoord) * pin.color;

    float4 color = texture_maps[0].Sample(sampler_states[ANISOTROPIC],pin.texcoord);
    //float alpha = color.a;
#if 1
    // Inverse gamma process
    const float GAMMA = 1.0;
    //const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);
#endif

    float3 N = normalize(pin.world_normal.xyz);
    float3 T = normalize(pin.world_tangent.xyz);
    float sigma = pin.world_tangent.w;
    T = normalize(T - N * dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);

    float4 normal = texture_maps[1].Sample(sampler_states[LINEAR], pin.texcoord);
    normal = (normal * 2.0) - 1.0;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));

    float3 L = normalize(-light_direction.xyz);
    float3 diffuse = color.rgb * max(0, dot(N, L));
    float3 V = normalize(camera_position.xyz - pin.world_position.xyz);
    float3 specular = pow(max(0, dot(N, normalize(V + L))), 128);

    // dissolve
    float alpha = step(parameters.x, mask_value);


    //color.a *= alpha;
    //return color;


    return color * pin.color;
    //return float4(diffuse, alpha) * pin.color;

    return float4(diffuse + specular, alpha) * pin.color;
}
#endif