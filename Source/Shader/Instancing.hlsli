struct VertexInput
{
    float4 position : POSITION;
    row_major float4x4 world : WORLD;
    float4 color : COLOR;
};

struct VertexOutput
{
    float4 sv_position : SV_POSITION;
    float4 color : COLOR;
};

struct PixelOutput
{
    float4 color : SV_TARGET;
};

cbuffer ConstantBuffer : register(b0)
{
    row_major float4x4 view_projection;
}

VertexOutput VSMain(VertexInput vin)
{
    VertexOutput vout;
    vout.sv_position = mul(mul(vin.position, vin.world), view_projection);

    return vout;
}

PixelOutput PSMain(VertexOutput pin)
{
    PixelOutput pout = (PixelOutput)0;

    pout.color = pin.color;
    
    return pout;
}