
struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

cbuffer Window : register(b0) {
	float4 windowSize;
};

SamplerState texSampler : register(s0);

Texture2D blurTexture : register(t0);


float4 main(VertexOut IN) : SV_TARGET {

	float3 color;
	for (int i = -2; i <= 2; ++i) {
		for (int j = -2; j <= 2; ++j) {
			float2 offset = IN.texCoord + float2(i, j) * float2(1.0 / windowSize.xy);
			color += blurTexture.Sample(texSampler, offset).rgb;
		}
	}
	color /= 25.0;

	return float4(color, 1.0);
}