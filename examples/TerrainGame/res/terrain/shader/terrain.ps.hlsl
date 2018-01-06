
struct input {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(input IN) : SV_TARGET {
	float4 OUT;

	OUT = float4(1.0, 0.0, 0.0, 1.0);
	
	return OUT;
}