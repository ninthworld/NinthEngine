struct input {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float blend : BLENDWEIGHT0;
};

cbuffer Skydome : register(b1) {
	float4 skyColor;
	float4 scale;
};

Texture2D skyTexture : register(t0);
SamplerState skySampler : register(s0);

float4 mix(float4 x, float4 y, float a) {
	return x * (1.0 - a) + y * a;
}

float4 main(input IN) : SV_TARGET {
	float4 OUT;

	float4 skyTex = skyTexture.Sample(skySampler, IN.texCoord);

	OUT = mix(skyTex, skyColor, IN.blend);
	
	return OUT;
}