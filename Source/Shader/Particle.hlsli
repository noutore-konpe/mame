cbuffer PARTICLE_CONSTANTS : register(b9)
{
    float3 emitterPosition; // emitter: �����f�q
    float particleSize;
    float4 particleColor;
    float time;
    float deltaTime;
    
    float2 scrollDirection; // �X�N���[������
    float options;          // �^�C��
    
    float2 dissolveParameters; // x : �f�B�]���u�K���ʁA y : ����臒l
    float something; // �_�~�[
    
    float4 edgeColor;   // ���̐F
};

cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 viewProjection;
    float4 lightDirection;
    float4 cameraPosition;
};

struct VS_OUT
{
    uint vertexId : VERTEXID;
};

struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;    
    
    float2 scrollTexcoord : TEXCOORD;
};

struct Particle
{
    float4 color;
    float3 position;
    float3 velocity;
    float age;
    int state;
};

#define NUMTHREADS_X 16