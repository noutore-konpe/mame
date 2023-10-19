SamplerState samplerState : register(s0);
Texture2D textureMaps[2] : register(t0);

float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    float4 SpriteColor = textureMaps[0].Sample(samplerState, texcoord);
    float4 ModelColor = textureMaps[1].Sample(samplerState, texcoord);
	
    float4 color = SpriteColor * ModelColor;
    
    return float4(color.rgb, 1.0f);
}