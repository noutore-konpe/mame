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

//////////////////////////////////////////////////
//                  �֐��錾                     //
//////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcLightFromDirectionLight(VS_OUT psIn);
float3 CalcLightFromPointLight(VS_OUT psIn);
float3 CalcLightFormSpotLight(VS_OUT psIn);

float4 main(VS_OUT psIn) : SV_TARGET
{
    float mask_value = maskTexture.Sample(samplerStates[0], psIn.texcoord) * psIn.color;

    float4 color = textureMaps[0].Sample(samplerStates[ANISOTROPIC], psIn.texcoord);
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
    // todo : 256�̕�����constant�ő��삵����
    float3 V = normalize(cameraPosition.xyz - psIn.worldPosition.xyz);
    float3 specular = pow(max(0, dot(N, normalize(V + L))), 256);
    // SPECULAR
    specular *= 0.1;
#else
    // ������������{�ɍ���Ă݂�
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
    // �����ł́A�T�[�t�F�X�̖@���ƌ��̕����Ɋ�Â��āA�o�C�A�X�̍ő�l�� 0.01�A�ŏ��l�� 0.001 �ɂȂ��Ă��܂��B
    const float shadowDepthBias = max(0.01 * (1.0 - dot(N, L)), 0.001);
#endif

    float4 lightViewPosition = mul(psIn.worldPosition, lightViewProjection);
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
    float3 pointLight = CalcLightFromPointLight(psIn);

    // SPOT_LIGHT
    float3 spotLight = CalcLightFormSpotLight(psIn);
    

    // �����ł̓��f���̓񎟔��ˌ����^���I�ɏo��
#if 1
    // float3 finalColor = diffuse + specular;
    // �|�C���g���C�g�l��
    float3 finalColor = directionLight + pointLight + spotLight;
#else
    float3 finalColor = diffuse;
#endif
    finalColor.x += 0.2f;
    finalColor.y += 0.2f;
    finalColor.z += 0.2f;

    return float4(finalColor * shadowFactor, alpha) * psIn.color;

    //return float4((diffuse + specular) * shadowFactor/*SHADOW*/, alpha) * psIn.color;

    //color.a *= alpha;
    //return color;
    

    //return color * psIn.color;
    //return float4(diffuse, alpha) * psIn.color;
    
    //return float4(diffuse + specular, alpha) * psIn.color;
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

// �f�B���N�V�������C�g�ɂ�锽�ˌ����v�Z
float3 CalcLightFromDirectionLight(VS_OUT psIn)
{
    // �f�B���N�V�������C�g�ɂ��Lambert�g�U���ˌ����v�Z����
    float3 diffuseDirection = CalcLambertDiffuse(
        directionLig.direction.xyz,
        directionLig.color.xyz,
        psIn.worldNormal.xyz
    );

    // �f�B���N�V�������C�g�ɂ��Phong���ʔ��ˌ����v�Z����
    float3 specularDirection = CalcPhongSpecular(
        directionLig.direction.xyz,
        directionLig.color.xyz,
        psIn.worldPosition.xyz,
        psIn.worldNormal.xyz
    );
    
    return diffuseDirection + specularDirection;
}

// �|�C���g���C�g�ɂ�锽�ˌ����v�Z
float3 CalcLightFromPointLight(VS_OUT psIn)
{   
    // ���̃T�[�t�F�C�X�ɓ��˂��Ă���|�C���g���C�g�̌��̌������v�Z����
    float3 lightDirection = psIn.worldPosition.xyz - pointLig.position.xyz;
        
    // ���K�����đ傫��1�̃x�N�g���ɂ���
    lightDirection = normalize(lightDirection);

    // �����Ȃ���Lambert�g�U���ˌ����v�Z����
    float3 diffusePoint = CalcLambertDiffuse(
        lightDirection,         // ���C�g�̕���
        pointLig.color.xyz, // ���C�g�̐F
        psIn.worldNormal.xyz    // �T�[�t�F�C�X�̖@��
    );
        
    // �����Ȃ���Phong���ʔ��ˌ����v�Z����
    float3 specularPoint = CalcPhongSpecular(
        lightDirection,         // ���C�g�̕���
        pointLig.color.xyz, // ���C�g�̃J���[
        psIn.worldPosition.xyz, // �T�[�t�F�C�X�̃��[���h���W
        psIn.worldNormal.xyz    // �T�[�t�F�C�X�̖@��
    );

    // �����ɂ��e�������v�Z����
    {
        // �|�C���g���C�g�Ƃ̋������v�Z����
        float distance = length(psIn.worldPosition.xyz - pointLig.position.xyz);
        
        // �e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
        float affect = 1.0f - 1.0f / pointLig.range * distance;
        
        // �e���͂��}�C�i�X�ɂȂ�Ȃ��悤�ɏC����������
        affect = max(0.0f, affect);
        
        // �e�����w���֐��I�ɂ���B�����3�悵�Ă�
        affect = pow(affect, 3.0f);
        
        // �g�U���ˌ��Ƌ��ʔ��ˌ��ɉe��������Z���ĉe������߂�
        diffusePoint *= affect;
        specularPoint *= affect;
    }
    
    return diffusePoint + specularPoint;
}

// �X�|�b�g���C�g�ɂ�锽�ˌ����v�Z
float3 CalcLightFormSpotLight(VS_OUT psIn)
{
    // ���̃T�[�t�F�C�X�ɓ��˂���X�|�b�g���C�g�̌��̌������v�Z����
    float3 lightDirection = psIn.worldPosition.xyz - spotLig.position.xyz;
    
    // ���K�����đ傫���P�̃x�N�g���ɂ���
    lightDirection = normalize(lightDirection);
    
    // �����Ȃ���Lambert�g�U���ˌ����v�Z����
    float3 diffuseSpotLight = CalcLambertDiffuse(
        lightDirection,         // ���C�g�̕���
        spotLig.color.xyz, // ���C�g�̐F
        psIn.worldPosition.xyz  // �T�[�t�F�C�X�̖@��
    );

    // �����Ȃ���Phong���ʔ��ˌ����v�Z����
    float3 specularSpotLight = CalcPhongSpecular(
        lightDirection,         // ���C�g�̕���
        spotLig.color.xyz, // ���C�g�̃J���[
        psIn.worldPosition.xyz, // �T�[�t�F�C�X�̃��[���h���W
        psIn.worldNormal.xyz    // �T�[�t�F�C�X�̖@��
    );

    // �����ɂ�錸�������v�Z����
    {
        // �X�|�b�g���C�g�Ƃ̋������v�Z����
        float3 distance = length(psIn.worldPosition.xyz - spotLig.position.xyz);
        
        // �e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
        float affect = 1.0f - 1.0f / spotLig.range * distance;
        
        // �e���͂��}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
        affect = max(0.0f, affect);
        
        // �e���̎d�����w���֐��I�ɂ���B����͂R�悵�Ă���
        affect = pow(affect, 3.0f);
        
        // �e��������Z���Ĕ��ˌ�����߂�
        diffuseSpotLight *= affect;
        specularSpotLight *= affect;
    }   
    
    // �p�x�ɂ��e���������߂�
    {
        // ���ˌ��Ǝˏo�����̊p�x�����߂�
        float angle = dot(lightDirection, spotLig.direction);
        angle = abs(acos(angle));
       
        // �p�x�ɔ�Ⴕ�ď������Ȃ��Ă����e�������v�Z����
        float affect = 1.0f - 1.0f / spotLig.angle * angle;
        
        // �e�������}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
        affect = max(0.0f, affect);
        
        // �e���̎d�����w���֐��I�ɂ���B�����0.5�悵�Ă���
        diffuseSpotLight *= affect;
        specularSpotLight *= affect;
    }

    return diffuseSpotLight + specularSpotLight;
}