#define POINT_LIGHT_ONE 1

struct VSIn
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 boneWeights : WEIGHTS;
    uint4 boneIndices : BONES;
};

struct PSIn
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 worldNormal : NORMAL;
    float4 worldTangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
    
    // LIM_LIGHT
    float3 normalInView : TEXCOORD2;
    
    // ZELDA
    float3 localPosition : LOCAL_POSITION;
    
    // EMISSIVE
    float2 emissiveTexcoord : TEXCOORD3;
};

// ���f�����
static const int MAX_BONES = 256;
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    float4 materialColor;
    row_major float4x4 boneTransforms[MAX_BONES];
    
    // EMISSIVE
    float4 emissiveColor;
    float emissiveIntensity;
    float emissiveOptions;
    float2 emissiveScrollDirection;    
    
    float4 sageColor; // ZELDA
};

// �J�����ʒu�A�f�B���N�V�������C�g���
cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 viewProjection;
    float4 lightDirection;
    float4 cameraPosition;
    row_major float4x4 lightViewProjection;
};


// �f�B���N�V�������C�g
struct DirectionLight
{
    float4 direction; // ���C�g�̕���
    float4 color; // ���C�g�̐F
};

// �|�C���g���C�g
struct PointLight
{
    float4 position; // �|�C���g���C�g�̈ʒu
    float4 color; // �|�C���g���C�g�̐F
    float range; // �|�C���g���C�g�̉e���͈�
    float3 step; // �_�~�[
};

// �X�|�b�g���C�g
struct SpotLight
{
    float4 position; // �X�|�b�g���C�g�̈ʒu
    float4 color; // �X�|�b�g���C�g�̐F
    float range; // �X�|�b�g���C�g�̉e���͈�
    float3 step; // �_�~�[
    float3 direction; // �X�|�b�g���C�g�̎ˏo����
    float angle; // �X�|�b�g���C�g�̎ˏo�p�x
};

// �������C�g
struct HemisphereLight
{
    float4 groundColor; // �Ƃ�Ԃ��̃��C�g
    float4 skyColor; // �V�����C�g
    float4 groundNormal; // �n�ʂ̖@��
};

// ���C�g
cbuffer LIGHT_CONSTANT_BUFFER : register(b5)
{
    DirectionLight directionLig; // �f�B���N�V�������C�g
    
#if POINT_LIGHT_ONE
    PointLight pointLig;            // �|�C���g���C�g
#else
    PointLight pointLig[8];
#endif
    
    SpotLight spotLig; // �X�|�b�g���C�g
    HemisphereLight hemisphereLig; // �������C�g
}

cbuffer DISSOLVE_CONSTANT_BUFFER : register(b3)
{
    float4 parameters;
}

///////////////////////////////////
// ---------- �e��֐� ---------- //
///////////////////////////////////

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
float3 CalcLightFromDirectionLight(PSIn psIn)
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
    
    // �g�U���ˌ��𐳋K������
    diffuseDirection /= 3.1415926f;
    
    return diffuseDirection + specularDirection;
}

//float3 ClacLightFromDirectionLight

// �|�C���g���C�g�ɂ�锽�ˌ����v�Z
#if POINT_LIGHT_ONE
float3 CalcLightFromPointLight(PSIn psIn)
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
#else
float3 CalcLightFromPointLight(PSIn psIn, PointLight p)
{
     // ���̃T�[�t�F�C�X�ɓ��˂��Ă���|�C���g���C�g�̌��̌������v�Z����
    float3 lightDirection = psIn.worldPosition.xyz - p.position.xyz;
        
    // ���K�����đ傫��1�̃x�N�g���ɂ���
    lightDirection = normalize(lightDirection);

    // �����Ȃ���Lambert�g�U���ˌ����v�Z����
    float3 diffusePoint = CalcLambertDiffuse(
        lightDirection, // ���C�g�̕���
        p.color.xyz, // ���C�g�̐F
        psIn.worldNormal.xyz    // �T�[�t�F�C�X�̖@��
    );
        
    // �����Ȃ���Phong���ʔ��ˌ����v�Z����
    float3 specularPoint = CalcPhongSpecular(
        lightDirection, // ���C�g�̕���
        p.color.xyz, // ���C�g�̃J���[
        psIn.worldPosition.xyz, // �T�[�t�F�C�X�̃��[���h���W
        psIn.worldNormal.xyz    // �T�[�t�F�C�X�̖@��
    );

    // �����ɂ��e�������v�Z����
    {
        // �|�C���g���C�g�Ƃ̋������v�Z����
        float distance = length(psIn.worldPosition.xyz - p.position.xyz);
        
        // �e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
        float affect = 1.0f - 1.0f / p.range * distance;
        
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
#endif// POINT_LIGHT_ONE

// �X�|�b�g���C�g�ɂ�锽�ˌ����v�Z
float3 CalcLightFormSpotLight(PSIn psIn)
{
    // ���̃T�[�t�F�C�X�ɓ��˂���X�|�b�g���C�g�̌��̌������v�Z����
    float3 lightDirection = psIn.worldPosition.xyz - spotLig.position.xyz;
    
    // ���K�����đ傫���P�̃x�N�g���ɂ���
    lightDirection = normalize(lightDirection);
    
    // �����Ȃ���Lambert�g�U���ˌ����v�Z����
    float3 diffuseSpotLight = CalcLambertDiffuse(
        lightDirection, // ���C�g�̕���
        spotLig.color.xyz, // ���C�g�̐F
        psIn.worldPosition.xyz  // �T�[�t�F�C�X�̖@��
    );

    // �����Ȃ���Phong���ʔ��ˌ����v�Z����
    float3 specularSpotLight = CalcPhongSpecular(
        lightDirection, // ���C�g�̕���
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

// �������C�g
float3 CalcLimLight(PSIn psIn)
{
    // �T�[�t�F�C�X�̖@���ƌ��̓��˕����Ɉˑ����郊���̋��������߂�
    float power1 = 1.0f - max(0.0f, dot(directionLig.direction, psIn.worldNormal));

    // �T�[�t�F�C�X�̖@���Ǝ��_�̕����Ɉˑ����郊���̋��������߂�
    float power2 = 1.0f - max(0.0f, psIn.normalInView * -1.0f);
    
    // �ŏI�I�ȃ����̋��������߂�
    float limPower = power1 * power2;
    
    // �����̕ω����w���֐��I�ɂ���
    limPower = pow(limPower, 1.3f);
    
    return limPower * directionLig.color;
}

// �������C�g
float3 CalcHemisphereLight(PSIn psIn)
{
    // �T�[�t�F�C�X�̖@���ƒn�ʂ̖@���Ƃ̓��ς��v�Z����
    float t = dot(psIn.worldNormal, hemisphereLig.groundNormal);

    // ���ς̌��ʂ��O�`�P�͈̔͂ɕϊ�����
    t = (t + 1.0f) / 2.0f;
    
    // �n�ʐF�ƓV���F���ԗ����Ő��`��Ԃ���
    float3 hemisphereLight = lerp(hemisphereLig.groundColor, hemisphereLig.skyColor, t);
        
    return hemisphereLight;
}