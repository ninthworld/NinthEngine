cbuffer ViewProjMatrix : register(b0) {
	matrix viewProjMatrix;
}

cbuffer ModelMatrix : register(b1) {
	matrix modelMatrix;
}

struct input {
	float3 position : POSITION;
	float3 color : COLOR0;
	float2 texCoord : TEXCOORD0;
};

struct output {
	float4 position : SV_POSITION;
	float3 color : COLOR0;
	float2 texCoord : TEXCOORD0;
};

output main(input IN) {
	output OUT;

	OUT.position = mul(viewProjMatrix, mul(modelMatrix, float4(IN.position, 1.0f)));
	OUT.color = IN.color;
	OUT.texCoord = IN.texCoord;

	//OUT.position = float4(IN.position, 1.0f);

	return OUT;
}