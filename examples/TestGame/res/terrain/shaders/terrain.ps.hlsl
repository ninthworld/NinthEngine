struct input {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

float4 main(input IN) : SV_TARGET {
	float4 OUT;

	OUT = float4(1, 0, 0, 1) * dot(IN.normal, normalize(float3(1, 1, 1)));
	
	return OUT;
}