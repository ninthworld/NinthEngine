
struct input {
	float4 position : SV_POSITION;
	float3 color : COLOR0;
};

float4 main(input IN) : SV_TARGET {

	return float4(IN.color, 1.0);
}