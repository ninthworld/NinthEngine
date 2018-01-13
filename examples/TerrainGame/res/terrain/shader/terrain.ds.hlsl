
struct HullOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct HullPatchOut {
    float edges[4] : SV_TessFactor;
    float inside[2]  : SV_InsideTessFactor;
};

struct DomainOut {
	float2 texCoord : TEXCOORD0;
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

SamplerState texSampler : register(s0);

Texture2D heightmap : register(t0);

[domain("quad")]
DomainOut main(
	HullPatchOut PATCH,
	float2 UV : SV_DomainLocation,
	const OutputPatch<HullOut, 4> IN) {
	DomainOut OUT;

	float2 posTopMid = lerp(IN[0].svPosition.xz, IN[1].svPosition.xz, UV.x);
	float2 posBotMid = lerp(IN[2].svPosition.xz, IN[3].svPosition.xz, UV.x);

	float2 texCoordTopMid = lerp(IN[0].texCoord, IN[1].texCoord, UV.x);
	float2 texCoordBotMid = lerp(IN[2].texCoord, IN[3].texCoord, UV.x);

	float2 pos = lerp(posBotMid, posTopMid, UV.y);
	float4 position = float4(pos.x, 0, pos.y, 1);

	float2 texCoord = lerp(texCoordBotMid, texCoordTopMid, UV.y);
	
	float height = 0;
	height += heightmap.SampleLevel(texSampler, texCoord, 0).r;
	height *= scaleY;

	position.y = height;

	OUT.texCoord = texCoord * 1000; // TODO: Replace with texDetail
	OUT.svPosition = position;

	return OUT;
}