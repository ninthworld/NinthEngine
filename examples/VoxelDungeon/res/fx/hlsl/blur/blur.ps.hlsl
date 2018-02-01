
struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct PixelOut {
	float4 color : SV_TARGET0;
};

cbuffer Window : register(b0) {
	float4 windowSize;
};

SamplerState texSampler : register(s0);

Texture2D colorTexture : register(t0);

PixelOut main(VertexOut IN) {
	PixelOut OUT;

	float3 color = float3(0, 0, 0);
	for (int i = -2; i <= 2; ++i) {
		for (int j = -2; j <= 2; ++j) {
			float2 offset = IN.texCoord + float2(i, j) * float2(1.0 / windowSize.xy);
			color += colorTexture.Sample(texSampler, offset).rgb;
		}
	}
	color /= 25.0;

	OUT.color = float4(color, 1.0);

	return OUT;
}