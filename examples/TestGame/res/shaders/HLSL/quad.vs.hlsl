struct input {
	float2 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct output {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

output main(input IN) {
	output OUT;

	OUT.position = float4(IN.position, 0.0f, 1.0f);
	OUT.texCoord = IN.texCoord;

	return OUT;
}