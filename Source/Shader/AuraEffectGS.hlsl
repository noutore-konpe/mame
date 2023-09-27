#include "AuraEffect.hlsli"

[maxvertexcount(4)]
void main(point VS_OUT input[1],inout TriangleStream<GS_OUT> output)
{
    const float3 billboard[] =
    {
        float3(-1.0f, -0.0f, 0.0f), // Bottom-left corner
        float3(+1.0f, -0.0f, 0.0f), // Bottom-right corner
        float3(-1.0f, +1.0f, 0.0f), // Top-left corner
        float3(+1.0f, +1.0f, 0.0f), // Top-right corner
    };

    const float2 texcoords[] =
    {
        float2(0.0f, 1.0f), // Bottom-left
        float2(1.0f, 1.0f), // Bottom-right
        float2(0.0f, 0.0f), // Top-left
        float2(1.0f, 0.0f), // Top-right
    };
    
    float3 position = input[0].position.xyz;
    
    [unroll]
    for (uint vertexIndex = 0; vertexIndex < 4;++vertexIndex)
    {
        GS_OUT element;
        
        float3 cornerPos = billboard[vertexIndex] * float3(0.25, 0.8, 0.0);
        element.position = mul(float4(position + cornerPos, 1), viewProjection);
        element.texcoord = texcoords[vertexIndex];
        element.texcoord.x = (element.texcoord.x + input[0].type) / 5.0;
        
        element.color = float4(1.0, 1.0, 1.0, 1);
        output.Append(element);
    }
    output.RestartStrip();
}