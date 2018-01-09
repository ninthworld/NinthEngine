
struct VertexIn {
	float2 position : POSITION;
};

struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

cbuffer Terrain : register(b1) {
	float4x4 worldMatrix;
	float scaleXZ;
	float scaleY;
	float detailRangeNear;
	float detailRangeFar;
	float lodRange[8];
};

cbuffer Node : register(b2) {
	float4x4 localMatrix;
	float4 neighbors;
	float2 location;
	float2 index;
	float size;
	int lod;
	float2 _nPadding;
};

SamplerState texSampler : register(s0);

Texture2D heightmap : register(t0);

VertexOut main(VertexIn IN) {
	VertexOut OUT;
	
	float2 localPos = mul(localMatrix, float4(IN.position.x, 0.0, IN.position.y, 1.0)).xz;

	OUT.texCoord = localPos;

	float height = 0.0;
	height += heightmap.SampleLevel(texSampler, OUT.texCoord, 0).r;
	
	OUT.svPosition = mul(worldMatrix, float4(localPos.x, height, localPos.y, 1.0));

	return OUT;
}