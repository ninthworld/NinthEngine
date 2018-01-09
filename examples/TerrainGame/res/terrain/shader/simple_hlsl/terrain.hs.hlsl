
struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct HullOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct HullPatchOut {
    float svEdges[4] : SV_TessFactor;
    float svInside[2]  : SV_InsideTessFactor;
};

HullPatchOut mainPatch(
	InputPatch<VertexOut, 16> IN, 
	uint id : SV_PrimitiveID) {
	HullPatchOut OUT;
	
	OUT.svEdges[0] = OUT.svEdges[1] = OUT.svEdges[2] = OUT.svEdges[3] = 16;
	OUT.svInside[0] = OUT.svInside[1] = 16;

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
	
	OUT.texCoord = IN[id].texCoord;
	OUT.svPosition = IN[id].svPosition;

	return OUT;
}