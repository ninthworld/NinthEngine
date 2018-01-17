
struct VertexOut {
	float3 normal	: NORMAL;
	float2 texCoord : TEXCOORD;
	float alpha		: BLEND;
	float4 svPosition : SV_POSITION;
};

struct PixelOut {
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
};

SamplerState texSampler : register(s0);

Texture2D grassColor : register(t0);
Texture2D grassAlpha : register(t1);

PixelOut main(VertexOut IN) {
	PixelOut OUT;
	
	float3 color = grassColor.Sample(texSampler, IN.texCoord).rgb;
	float alpha = grassAlpha.Sample(texSampler, IN.texCoord).r;

	OUT.normal = float4(IN.normal, 1);
	OUT.color = float4(color, alpha * IN.alpha);

	return OUT;	
}