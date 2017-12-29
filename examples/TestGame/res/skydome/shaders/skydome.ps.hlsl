struct input {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

Texture2D skyTexture : register(t0);
SamplerState skySampler : register(s0);

float4 main(input IN) : SV_TARGET {
	float4 OUT;
	
	OUT = skyTexture.Sample(skySampler, IN.texCoord);
	
	return OUT;
}