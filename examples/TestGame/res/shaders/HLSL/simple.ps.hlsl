Texture2D blockTexture : register(t0);
SamplerState blockSampler : register(s0);

struct input {
	float4 position : SV_POSITION;
	float3 color : COLOR0;
	float2 texCoord : TEXCOORD0;
};

float4 main(input IN) : SV_TARGET {

	float4 OUT = blockTexture.Sample(blockSampler, IN.texCoord);
	
	return OUT;
}