
struct VertexIn {
	float2 position : POSITION;
};

struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

VertexOut main(VertexIn IN) {
	VertexOut OUT;
	
	OUT.texCoord = (float2(IN.position.x, -IN.position.y) + 1.0) / 2.0;
	OUT.svPosition = float4(IN.position, 0.0, 1.0);

	return OUT;
}