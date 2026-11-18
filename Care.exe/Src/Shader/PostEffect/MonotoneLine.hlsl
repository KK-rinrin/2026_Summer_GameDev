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
    
    float4 dstColor = { 1, 1, 1, 1 };
    
    // 縞模様
    dstColor.rgb *= sin(150.0f * psInput.uv.y - totalTime * 5.0f);
    dstColor.rgb *= 0.2f; // 濃さ
    dstColor.rgb += 0.8f; // 明るさ
    
    dstColor *= srcColor;
    
        // モノクロ（色平均化）
    dstColor = (dstColor.r + dstColor.g + dstColor.b) / 3.0f;
    
	return dstColor;
}