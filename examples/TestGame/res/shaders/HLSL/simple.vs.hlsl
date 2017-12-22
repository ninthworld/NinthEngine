cbuffer ViewProjMatrix : register(b0) {
	matrix viewProjMatrix;
}

cbuffer ModelMatrix : register(b1) {
	matrix modelMatrix;
}

struct input {
	float3 position : POSITION;
	float3 color : COLOR;
};

struct output {
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

output main(input IN) {
	output OUT;

	OUT.position = mul(viewProjMatrix, mul(modelMatrix, float4(IN.position, 1.0f)));
	OUT.color = IN.color;
	//OUT.position = float4(IN.position, 1.0f);

	return OUT;
}