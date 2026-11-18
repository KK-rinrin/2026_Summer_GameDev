#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4
cbuffer cbParam : register(b4)
{
    float4 g_color;
    float g_progress;
    float2 dummy1;
};


float4 main(PS_INPUT psInput) : SV_TARGET
{
    // テクスチャから色を取り出す
    float4 srcColor = tex.Sample(texSampler, psInput.uv);
    
    // -0.5～+0.5
    // ノイズ計算
    float noise = frac(sin(
		dot(psInput.uv * g_progress, float2(12.9898f, 78.233f))) * 43758.5453f) - 0.5f;
    
    float4 dstColor = srcColor;
    
    // ノイズカラーを加算する
    dstColor.rgb += float3(noise, noise, noise) * 0.3f;
    
    return dstColor;
}