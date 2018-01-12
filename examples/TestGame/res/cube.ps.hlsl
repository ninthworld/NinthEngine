
struct VertexOut {
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

struct PixelOut {
	float4 pixelColor : SV_TARGET0;
};

PixelOut main(VertexOut IN) {
	PixelOut OUT;

	OUT.pixelColor = float4(IN.color, 1.0);

	return OUT;
}