
struct VertexOut {
	float4 svPosition : SV_POSITION;
};

struct PixelOut {
	float4 color : SV_TARGET;
};

PixelOut main(VertexOut IN) {
	PixelOut OUT;
	
	OUT.color = float4(0.0, 0.0, 0.0, 1.0);

	return OUT;
}