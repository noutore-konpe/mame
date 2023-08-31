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
};

// モデル情報
static const int MAX_BONES = 256;
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    float4 materialColor;
    float4 dummy;
    float4 dummy2;
    row_major float4x4 boneTransforms[MAX_BONES];
};

// カメラ位置、ディレクションライト情報
cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 viewProjection;
    float4 lightDirection;
    float4 cameraPosition;
    // SHADOW
    row_major float4x4 lightViewProjection;
};

// ポイントライト
cbuffer POINT_LIGHT_CONSTANT_BUFFER : register(b5)
{
    float3 ptPosition;    // ポイントライトの位置
    float step;        // ダミー
    float3 ptColor;       // ポイントライトの色
    float  ptRange;       // ポイントライトの影響範囲
}

cbuffer DISSOLVE_CONSTANT_BUFFER : register(b3)
{
    float4 parameters;
}