
struct VertexOut {
	float3 color : COLOR;
	float3 normal : NORMAl;
	float4 svPosition : SV_POSITION;
};

struct PixelOut {
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
};

PixelOut main(VertexOut IN) {
	PixelOut OUT;

	OUT.color = float4(IN.color, 1.0);
	OUT.normal = float4(IN.normal, 1.0);

	return OUT;
}