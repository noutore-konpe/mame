#include "Particle.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState samplerState[3] : register(s0);

// TODO : particle
// ���ŁAemissiveNoiseTexture�g���Ă�B��ŕς���
Texture2D textureMap : register(t16);

float4 main(GS_OUT psIn) : SV_TARGET
{
    float4 color = textureMap.Sample(samplerState[POINT], psIn.texcoord);
    //float4 color = textureMap.Sample(samplerState[POINT], psIn.scrollTexcoord);
    
    // dissolve
    {
        // �}�X�N�摜����ԐF���擾
        float mask = textureMap.Sample(samplerState[POINT], psIn.texcoord).r;

        // step�֐���p���ē��ߓx��0/1�ɂ���
        float alpha = step(dissolveParameters.x, mask);
        
        // ���̏���
        float edgeValue = step(mask - dissolveParameters.x, dissolveParameters.x) * step(dissolveParameters.x, mask) * step(mask, dissolveParameters.x + dissolveParameters.y);
        color.rgb += edgeColor.rgb * edgeValue;
        alpha = saturate(alpha + edgeValue);
        
        color.a *= alpha;
    }
    
    //color.rgb *= psIn.color.rgb;
    
    return color;
}