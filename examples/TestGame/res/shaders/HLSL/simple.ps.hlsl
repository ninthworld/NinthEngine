struct input {
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

float4 main(input IN) : SV_TARGET {
	float4 OUT = { IN.color, 1.0f }; //{ 1.0f, 0.0f, 0.0f, 1.0f };
	return OUT;
}