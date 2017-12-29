struct input {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct output {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

cbuffer ModelViewProjMatrix : register(b0) {
	matrix modelViewProjMatrix;
};

output main(input IN){
	output OUT;
	
	OUT.texCoord = IN.texCoord;
	OUT.position = mul(modelViewProjMatrix, float4(IN.position, 1.0f));
	
	return OUT;
}