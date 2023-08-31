#include "skinned_mesh.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState samplerStates[5] : register(s0);
Texture2D textureMaps[4] : register(t0);

// SHADOW
SamplerComparisonState comparisonSamplerState : register(s5);
Texture2D shadowMap : register(t8);

// DISSOLVE
Texture2D maskTexture : register(t15);

/////////////////////////
//      関数宣言        //
/////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);

float4 main(VS_OUT pin) : SV_TARGET
{
    float mask_value = maskTexture.Sample(samplerStates[0],pin.texcoord) * pin.color;

    float4 color = textureMaps[0].Sample(samplerStates[ANISOTROPIC],pin.texcoord);
    float alpha = color.a;

#if 1
    // 逆ガンマ補正
    //const float GAMMA = 1.0;
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);
#else
    // ガンマ補正
    const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, 1.0 / GAMMA);
#endif

    float3 N = normalize(pin.worldNormal.xyz);

#if 1
    float3 T = normalize(pin.worldTangent.xyz);
    float sigma = pin.worldTangent.w;
    T = normalize(T - N * dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);

    float4 normal = textureMaps[1].Sample(samplerStates[LINEAR], pin.texcoord);
    normal = (normal * 2.0) - 1.0;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));
#endif
    
    float3 L = normalize(-lightDirection.xyz);
    float3 diffuse = color.rgb * max(0, dot(N, L));
#if 1
    // todo : 256の部分をconstantで操作したい
    float3 V = normalize(cameraPosition.xyz - pin.worldPosition.xyz);
    float3 specular = pow(max(0, dot(N, normalize(V + L))), 256);
    // SPECULAR
    specular *= 0.1;
#else
    // 魔導書をお手本に作ってみた
    float3 refVec = reflect(L, N);
    float3 toEye = cameraPosition - pin.worldPosition;
    toEye = normalize(toEye);
    float3 specular = color.rgb * pow(max(0, dot(refVec, toEye)), 5.0f);
#endif

    // dissolve
#if 0
    alpha = step(parameters.x, mask_value);
#endif

    // SHADOW
#if 1
    const float shadowDepthBias = 0.01;
#else
    // ここでは、サーフェスの法線と光の方向に基づいて、バイアスの最大値が 0.01、最小値が 0.001 になっています。
    const float shadowDepthBias = max(0.01 * (1.0 - dot(N, L)), 0.001);
#endif

    float4 lightViewPosition = mul(pin.worldPosition, lightViewProjection);
    lightViewPosition = lightViewPosition / lightViewPosition.w;
    float2 lightViewTexcoord = 0;
    lightViewTexcoord.x = lightViewPosition.x * 0.5 + 0.5;
    lightViewTexcoord.y = lightViewPosition.y * -0.5 + 0.5;
    float depth = saturate(lightViewPosition.z - shadowDepthBias);
     
    
    // SHADOW
    float shadowWidth;
    float shadowHight;
    shadowMap.GetDimensions(shadowWidth, shadowHight);

    float shadowWidthOffset = 1 / shadowWidth;
    float shadowHightOffset = 1 / shadowHight;
    
    float3 shadowFactor = 1.0f;
    // shdowFactorひとつだけだと、まわりを取って平均値を出すことできれいに見せる。
    {
        shadowFactor = shadowMap.SampleCmpLevelZero(comparisonSamplerState, lightViewTexcoord, depth).xxx;
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y), depth).xxx; // 右
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y), depth).xxx; // 左
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // 上
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // 下
        
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // 右上
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y - shadowHightOffset), depth).xxx; // 左上
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x + shadowWidthOffset, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // 右下
        shadowFactor += shadowMap.SampleCmpLevelZero(comparisonSamplerState, float2(lightViewTexcoord.x - shadowWidthOffset, lightViewTexcoord.y + shadowHightOffset), depth).xxx; // 左下
        
        shadowFactor /= 9;
    }
    

    // POINT_LIGHT
    float3 light; // 最終的な結果を入れる
    {
        // サーフェイスに入射するポイントライトの光の向きを計算する
        float3 lightDirection = pin.worldPosition.xyz - ptPosition;
        
        // 正規化する
        lightDirection = normalize(lightDirection);

        // 減衰なしのLambert拡散反射光を計算する
        float3 diffusePoint = CalcLambertDiffuse(
            lightDirection,     // ライトの方向
            ptColor,            // ライトの色
            pin.worldNormal.xyz // サーフェイスの法線
        );
        
        // 減衰なしのPhong鏡面反射光を計算する
        float3 specularPoint = CalcPhongSpecular(
            lightDirection,         // ライトの方向
            ptColor,                // ライトのカラー
            pin.worldPosition.xyz,  // サーフェイスのワールド座標
            pin.worldNormal.xyz     // サーフェイスの法線
        );

        // ポイントライトとの距離を計算する
        float distance = length(pin.worldPosition.xyz - ptPosition);
        
        // 影響率は距離に比例して小さくなっていく
        float affect = 1.0f - 1.0f / ptRange * distance;
        
        // 影響力がマイナスにならないように修正をかける
        affect = max(0.0f, affect);
        
        // 影響を指数関数的にする。今回は3乗してる
        affect = pow(affect, 3.0f);
        
        // 拡散反射光と鏡面反射光に影響率を乗算して影響を弱める
        diffusePoint *= affect;
        specularPoint *= affect;
        
        // 2つの反射光を合算して最終的な反射光を求める
        float3 diffuseLight = diffusePoint + diffuse;
        float3 specularLight = specularPoint + specular;
        
        light = diffuseLight + specularLight;
    }
    

    // ここではモデルの二次反射光を疑似的に出す
#if 1
    // float3 finalColor = diffuse + specular;
    float3 finalColor = light;   // ポイントライト考慮
#else
    float3 finalColor = diffuse;
#endif
    finalColor.x += 0.2f;
    finalColor.y += 0.2f;
    finalColor.z += 0.2f;

    return float4(finalColor * shadowFactor, alpha) * pin.color;

    //return float4((diffuse + specular) * shadowFactor/*SHADOW*/, alpha) * pin.color;

    //color.a *= alpha;
    //return color;
    

    //return color * pin.color;
    //return float4(diffuse, alpha) * pin.color;
    
    //return float4(diffuse + specular, alpha) * pin.color;
}


// Lambert拡散反射光を計算する
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    // ピクセルの法線とライトの方向の内積を計算する
    float t = dot(normal, lightDirection) * -1.0f;
    
    // 内積の値を0以上の値にする
    t = max(0.0f, t);
    
    // 拡散反射光を計算する
    return lightColor * t;
}

// Phong鏡面反射光を計算する
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
    // 反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);
    
    // 光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = cameraPosition.xyz - worldPos;
    toEye = normalize(toEye);
    
    // 鏡面反射の強さを求める
    float t = dot(refVec, toEye);
    
    // 鏡面反射の強さを0以上にする
    t = max(0.0f, t);
    
    // 鏡面反射の強さを絞る
    t = pow(t, 5.0f);
    
    // 鏡面反射光を求める
    return lightColor * t;
}

#if 0
#include "skinned_mesh.hlsli"
#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3]:register(s0);
Texture2D texture_maps[4]:register(t0);
Texture2D mask_texture : register(t15);
float4 main(VS_OUT pin) :SV_TARGET
{
    float mask_value = mask_texture.Sample(sampler_states[0],pin.texcoord) * pin.color;

    float4 color = texture_maps[0].Sample(sampler_states[ANISOTROPIC],pin.texcoord);
    //float alpha = color.a;
#if 1
    // Inverse gamma process
    const float GAMMA = 1.0;
    //const float GAMMA = 2.2;
    color.rgb = pow(color.rgb, GAMMA);
#endif

    float3 N = normalize(pin.world_normal.xyz);
    float3 T = normalize(pin.world_tangent.xyz);
    float sigma = pin.world_tangent.w;
    T = normalize(T - N * dot(N, T));
    float3 B = normalize(cross(N, T) * sigma);

    float4 normal = texture_maps[1].Sample(sampler_states[LINEAR], pin.texcoord);
    normal = (normal * 2.0) - 1.0;
    N = normalize((normal.x * T) + (normal.y * B) + (normal.z * N));

    float3 L = normalize(-light_direction.xyz);
    float3 diffuse = color.rgb * max(0, dot(N, L));
    float3 V = normalize(camera_position.xyz - pin.world_position.xyz);
    float3 specular = pow(max(0, dot(N, normalize(V + L))), 128);

    // dissolve
    float alpha = step(parameters.x, mask_value);


    //color.a *= alpha;
    //return color;


    return color * pin.color;
    //return float4(diffuse, alpha) * pin.color;

    return float4(diffuse + specular, alpha) * pin.color;
}
#endif