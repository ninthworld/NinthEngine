
struct HullOut {
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct HullOutPatch {
    float edges[4] : SV_TessFactor;
    float inside[2]  : SV_InsideTessFactor;
};

struct DomainOut {
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

cbuffer Terrain : register(b1) {
	float4x4 worldMatrix;
	float scaleXZ;
	float scaleY;
	float detailRangeNear;
	float detailRangeFar;
	float lodRange[8];
};

SamplerState texSampler : register(s0);

Texture2D heightmap : register(t0);

[domain("quad")]
DomainOut main(
	HullOutPatch PATCH,
	float2 UV : SV_DomainLocation,
	const OutputPatch<HullOut, 16> IN) {
	DomainOut OUT;

	float u0 = UV.x;
	float v0 = UV.y;
	float u1 = (1 - u0);
	float v1 = (1 - v0);

	float4 position =
		u1 * v1 * IN[12].position +
		u0 * v1 * IN[0].position +
		u0 * v0 * IN[3].position +
		u1 * v0 * IN[15].position;

	float2 texCoord =
		u1 * v1 * IN[12].texCoord +
		u0 * v1 * IN[0].texCoord +
		u0 * v0 * IN[3].texCoord +
		u1 * v0 * IN[15].texCoord;

	float height = 0;
	height += heightmap.SampleLevel(texSampler, texCoord, 0).r;
	height *= scaleY;

	position.y = height;

	OUT.texCoord = texCoord * 1000; // TODO: Replace with texDetail
	OUT.position = position;

	return OUT;
}