Texture2D quadTexture : register(t0);
SamplerState quadSampler : register(s0);

struct input {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(input IN) : SV_TARGET {

	float4 OUT = quadTexture.Sample(quadSampler, IN.texCoord);
	
	return OUT;
}