
struct VertexOut {
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct HullOut {
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct HullOutPatch {
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
	return lerp(1, 16, float(_lod) / 8.0);
}

HullOutPatch mainPatch(
	InputPatch<VertexOut, 16> IN, 
	uint id : SV_PrimitiveID) {
	HullOutPatch OUT;

	float4 side = lerp4(tessLevel(lod), tessLevel(lod + 1), neighbors);

	OUT.edges[2] = side.x;
	OUT.edges[0] = side.y;
	OUT.edges[1] = side.z;
	OUT.edges[3] = side.w;

	OUT.inside[0] = (OUT.edges[3] + OUT.edges[1]) / 4.0;
	OUT.inside[0] = (OUT.edges[2] + OUT.edges[0]) / 4.0;

	return OUT;
}

[domain("quad")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(16)]
[patchconstantfunc("mainPatch")]
HullOut main(
	InputPatch<VertexOut, 16> IN, 
	uint id : SV_OutputControlPointID, 
	uint patchId : SV_PrimitiveID) {
	HullOut OUT;
	
	OUT.position = IN[id].position;
	OUT.texCoord = IN[id].texCoord;

	return OUT;
}