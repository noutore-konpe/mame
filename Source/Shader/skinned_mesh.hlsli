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

// ディレクションライト
struct DirectionLight
{
    float4 direction;   // ライトの方向
    float4 color;       // ライトの色
};

// ポイントライト
struct PointLight
{
    float4 position; // ポイントライトの位置
    float4 color;    // ポイントライトの色
    float range;     // ポイントライトの影響範囲
    float3 step;     // ダミー
};

// スポットライト
struct SpotLight
{
    float4 position;    // スポットライトの位置
    float4 color;       // スポットライトの色
    float range;        // スポットライトの影響範囲
    float3 step;        // ダミー
    float3 direction;   // スポットライトの射出方向
    float angle;        // スポットライトの射出角度
};

// 半球ライト
struct HemisphereLight
{
    float4 groundColor;     // 照り返しのライト
    float4 skyColor;        // 天球ライト
    float4 groundNormal;    // 地面の法線
};

// ライト
cbuffer LIGHT_CONSTANT_BUFFER : register(b5)
{
    DirectionLight directionLig;    // ディレクションライト
    PointLight pointLig;            // ポイントライト
    SpotLight spotLig;              // スポットライト
    HemisphereLight hemisphereLig;  // 半球ライト
}

cbuffer DISSOLVE_CONSTANT_BUFFER : register(b3)
{
    float4 parameters;
}