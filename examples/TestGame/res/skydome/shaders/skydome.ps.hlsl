struct input {
	float4 position : SV_POSITION;
	float3 vertex : POSITION;
	float2 texCoord : TEXCOORD0;
};

Texture2D skyTexture : register(t0);
SamplerState skySampler : register(s0);

float4 mix(float4 x, float4 y, float a) {
	return x * (1.0 - a) + y * a;
}

float4 main(input IN) : SV_TARGET {
	float4 OUT;

	OUT = mix(skyTexture.Sample(skySampler, IN.texCoord), float4(0.57, 0.67, 0.87, 1.0), (1.0 - IN.vertex.y) * (1.0 - IN.vertex.y));
	
	return OUT;
}