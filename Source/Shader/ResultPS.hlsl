cbuffer RESULT_CONSTANTS : register(b5)
{
    float4 color;
}

float4 main() : SV_TARGET
{
    float alpha = max(0, color.r);
    
    return float4(color.rgb, alpha);
}