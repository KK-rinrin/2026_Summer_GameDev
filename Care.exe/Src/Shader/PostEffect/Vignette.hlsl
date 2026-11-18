#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4
cbuffer cbParam : register(b4)
{
    float4 g_color;
    float totalTime;
    float3 dummy1;
};

float4 main(PS_INPUT psInput) : SV_TARGET
{
    // テクスチャから色を取り出す
    float4 srcColor = tex.Sample(texSampler, psInput.uv);
    
    // UVを中央にずらす
    float u = abs(psInput.uv.x - 0.5f);
    float v = abs(psInput.uv.y - 0.5f);
    
    // 真ん中から遠ければ白になる
    // 真ん中から近ければ黒になる
    float darkness = (u * u + v * v) * 2.0f;
    
    // ビネット効果（内側白、外側黒）
    float vignette = - darkness;
    
    // ビネット
    float4 dstColor = { 1, 1, 1, 1 };
    
    // 線形補間
    dstColor.rgb = lerp(srcColor.rgb, g_color.rgb, darkness);
    
    dstColor.a = srcColor.a;
    
    return dstColor;
    
}