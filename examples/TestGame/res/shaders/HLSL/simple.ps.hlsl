struct input {
	float4 position : SV_POSITION;
};

float4 main(input IN) : SV_TARGET {
	float4 OUT = { 1.0f, 0.0f, 0.0f, 1.0f };
	return OUT;
}