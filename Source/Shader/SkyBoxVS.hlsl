#include "SkyBox.hlsli"

VS_OUT main(float4 position : POSITION)
{
    VS_OUT vout;

    position.z = 1.0f;
    vout.position = position;

    // ���[���h�ϊ�
    float4 p = mul(position, inverseViewProjection);
    vout.worldPosition = p / p.w;
    
    return vout;
}