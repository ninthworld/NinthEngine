
struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct GeometryOut {
	float2 texCoord : TEXCOORD;
	float3 position : POSITION;
	float3 tangent : TANGENT;
	float4 svPosition : SV_POSITION;
};

cbuffer Camera : register(b0) {
	float4x4 camViewProj;
	float4 camPosition;
};

cbuffer Terrain : register(b1) {
	float4 terrainScale;
};

float3 calcTangent(VertexOut IN[3]) {

	float3 v0 = IN[0].svPosition.xyz;
	float3 v1 = IN[1].svPosition.xyz;
	float3 v2 = IN[2].svPosition.xyz;

	float3 e1 = v1 - v0;
	float3 e2 = v2 - v0;
	
	float2 uv0 = IN[0].texCoord;
	float2 uv1 = IN[1].texCoord;
	float2 uv2 = IN[2].texCoord;

	float2 deltaUV1 = uv1 - uv0;
	float2 deltaUV2 = uv2 - uv0;
	
	float r = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	
	return normalize((e1 * deltaUV2.y - e2 * deltaUV1.y) * r);
}

[maxvertexcount(3)]
void main(
	triangle VertexOut IN[3],
	inout TriangleStream<GeometryOut> stream) {
	
	for (int i = 0; i < 3; ++i) {
		GeometryOut OUT;

		OUT.texCoord = IN[i].texCoord * terrainScale.x;
		OUT.position = IN[i].svPosition.xyz;
		OUT.tangent = calcTangent(IN);
		OUT.svPosition = mul(camViewProj, IN[i].svPosition);

		stream.Append(OUT);
	}

	stream.RestartStrip();
}