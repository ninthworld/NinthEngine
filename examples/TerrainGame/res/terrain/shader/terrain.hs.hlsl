
struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct HullOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct HullPatchOut {
    float edges[4] : SV_TessFactor;
    float inside[2]  : SV_InsideTessFactor;
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

float4 lerp4(float a, float b, float4 x) {
	return float4(
			lerp(a, b, x.x), 
			lerp(a, b, x.y), 
			lerp(a, b, x.z), 
			lerp(a, b, x.w));
}

float tessLevel(int _lod) {
	return lerp(1, 64, float(_lod) / 8.0);
}

HullPatchOut mainPatch(
	InputPatch<VertexOut, 16> IN, 
	uint id : SV_PrimitiveID) {
	HullPatchOut OUT;

	float4 side = lerp4(tessLevel(lod), tessLevel(lod + 1) * 2, neighbors);

	OUT.edges[3] = side.x;
	OUT.edges[1] = side.y;
	OUT.edges[0] = side.z;
	OUT.edges[2] = side.w;

	OUT.inside[0] = (OUT.edges[3] + OUT.edges[1]) / 4.0;
	OUT.inside[1] = (OUT.edges[2] + OUT.edges[0]) / 4.0;

	return OUT;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(16)]
[patchconstantfunc("mainPatch")]
HullOut main(
	InputPatch<VertexOut, 16> IN, 
	uint id : SV_OutputControlPointID, 
	uint patchId : SV_PrimitiveID) {
	HullOut OUT;
	
	OUT.svPosition = IN[id].svPosition;
	OUT.texCoord = IN[id].texCoord;

	return OUT;
}