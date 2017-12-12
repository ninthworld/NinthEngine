cbuffer ViewProjMatrix : register(b0) {
	matrix viewProjMatrix;
}

cbuffer ModelMatrix : register(b1) {
	matrix modelMatrix;
}

struct input {
	float3 position : POSITION;
};

struct output {
	float4 position : SV_POSITION;
};

output main(input IN) {
	output OUT;

	matrix mvp = mul(viewProjMatrix, modelMatrix);
	OUT.position = mul(mvp, float4(IN.position, 1.0f));

	return OUT;
}