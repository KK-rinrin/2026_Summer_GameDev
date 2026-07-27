struct PS_INPUT
{
	float4 Diffuse    : COLOR0;
	float4 Specular   : COLOR1;
	float2 TexCoords0 : TEXCOORD0;
	float2 TexCoords1 : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 Output : SV_TARGET;
};

Texture2D SourceTexture : register(t0);
SamplerState SourceSampler : register(s0);

cbuffer ColorShiftConstants : register(b0)
{
	float4 ColorShiftOffset;
};

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	const float2 redUV = saturate(input.TexCoords0 - ColorShiftOffset.xy);
	const float2 blueUV = saturate(input.TexCoords0 + ColorShiftOffset.xy);
	const float4 centerColor = SourceTexture.Sample(SourceSampler, input.TexCoords0);

	output.Output.r = SourceTexture.Sample(SourceSampler, redUV).r;
	output.Output.g = centerColor.g;
	output.Output.b = SourceTexture.Sample(SourceSampler, blueUV).b;
	output.Output.a = centerColor.a;

	return output;
}
