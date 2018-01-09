
struct GeometryOut {
	float2 texCoord : TEXCOORD;
	float3 position : POSITION;
	float4 svPosition : SV_POSITION;
};

float4 main(GeometryOut IN) : SV_TARGET {
	float4 OUT;

	OUT = float4(1, 0, 0, 1);

	return OUT;
}