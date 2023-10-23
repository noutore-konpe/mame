cbuffer RESULT_CONSTANTS : register(b5)
{
    float4 color;
}

float4 main() : SV_TARGET
{
    return color;
}