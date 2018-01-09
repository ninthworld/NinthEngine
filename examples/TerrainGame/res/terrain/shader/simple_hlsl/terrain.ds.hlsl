
struct HullOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct HullPatchOut {
    float svEdges[4] : SV_TessFactor;
    float svInside[2]  : SV_InsideTessFactor;
};

struct DomainOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

[domain("quad")]
DomainOut main(
	HullPatchOut PATCH,
	float2 UV : SV_DomainLocation,
	const OutputPatch<HullOut, 16> IN) {
	DomainOut OUT;
	/*
	float u0 = UV.x;
	float v0 = UV.y;
	float u1 = (1 - u0);
	float v1 = (1 - v0);

	float4 position =
		u1 * v1 * IN[12].svPosition +
		u0 * v1 * IN[0].svPosition +
		u0 * v0 * IN[3].svPosition +
		u1 * v0 * IN[15].svPosition;

	float2 texCoord =
		u1 * v1 * IN[12].texCoord +
		u0 * v1 * IN[0].texCoord +
		u0 * v0 * IN[3].texCoord +
		u1 * v0 * IN[15].texCoord;

	OUT.texCoord = texCoord;
	OUT.svPosition = position;
	*/

	float4 topMid = lerp(IN[0].svPosition, IN[3].svPosition, UV.x);
	float4 botMid = lerp(IN[15].svPosition, IN[12].svPosition, UV.x);

	OUT.texCoord = IN[0].texCoord;
	OUT.svPosition = lerp(topMid, botMid, UV.y);

	return OUT;
}