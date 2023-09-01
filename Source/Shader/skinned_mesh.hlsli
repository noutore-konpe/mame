struct VS_IN
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 boneWeights : WEIGHTS;
    uint4 boneIndices : BONES;
};
struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 worldNormal : NORMAL;
    float4 worldTangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
    
    // LIM_LIGHT
    float3 normalInView : TEXCOORD2;
};

// ���f�����
static const int MAX_BONES = 256;
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    float4 materialColor;
    float4 dummy;
    float4 dummy2;
    row_major float4x4 boneTransforms[MAX_BONES];
};

// �J�����ʒu�A�f�B���N�V�������C�g���
cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 viewProjection;
    float4 lightDirection;
    float4 cameraPosition;
    // SHADOW
    row_major float4x4 lightViewProjection;
};

// �f�B���N�V�������C�g
struct DirectionLight
{
    float4 direction;   // ���C�g�̕���
    float4 color;       // ���C�g�̐F
};

// �|�C���g���C�g
struct PointLight
{
    float4 position; // �|�C���g���C�g�̈ʒu
    float4 color;    // �|�C���g���C�g�̐F
    float range;     // �|�C���g���C�g�̉e���͈�
    float3 step;     // �_�~�[
};

// �X�|�b�g���C�g
struct SpotLight
{
    float4 position;    // �X�|�b�g���C�g�̈ʒu
    float4 color;       // �X�|�b�g���C�g�̐F
    float range;        // �X�|�b�g���C�g�̉e���͈�
    float3 step;        // �_�~�[
    float3 direction;   // �X�|�b�g���C�g�̎ˏo����
    float angle;        // �X�|�b�g���C�g�̎ˏo�p�x
};

// �������C�g
struct HemisphereLight
{
    float4 groundColor;     // �Ƃ�Ԃ��̃��C�g
    float4 skyColor;        // �V�����C�g
    float4 groundNormal;    // �n�ʂ̖@��
};

// ���C�g
cbuffer LIGHT_CONSTANT_BUFFER : register(b5)
{
    DirectionLight directionLig;    // �f�B���N�V�������C�g
    PointLight pointLig;            // �|�C���g���C�g
    SpotLight spotLig;              // �X�|�b�g���C�g
    HemisphereLight hemisphereLig;  // �������C�g
}

cbuffer DISSOLVE_CONSTANT_BUFFER : register(b3)
{
    float4 parameters;
}