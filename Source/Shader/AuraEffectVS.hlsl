#include "AuraEffect.hlsli"

VS_OUT main(float4 position : POSITION, float type : TYPE)
{
    VS_OUT vout;
    vout.position = mul(position, world);
    vout.type = type;
    return vout;
}