cbuffer MVPMatrix : register(b0) {
	matrix mvpMatrix;
}

struct input {
	float3 position : POSITION;
};

struct output {
	float4 position : SV_POSITION;
};

output main(input IN) {
	output OUT;

	OUT.position = mul(mvpMatrix, float4(IN.position, 1.0f));

	//OUT.position = float4(IN.position, 1.0f);

	return OUT;
}