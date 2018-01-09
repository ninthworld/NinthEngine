
struct DomainOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

struct GeometryOut {
	float2 texCoord : TEXCOORD;
	float3 position : POSITION;
	float4 svPosition : SV_POSITION;
};

cbuffer Camera : register(b0) {
	float4x4 viewMatrix;
	float4x4 viewProjMatrix;
	float4 camPosition;
	float4 frustumPlanes[6];
};

[maxvertexcount(3)]
void main(
	triangle DomainOut IN[3],
	inout TriangleStream<GeometryOut> stream) {
	
	for (int i = 0; i < 3; ++i) {
		GeometryOut OUT;

		float4 worldPos = IN[i].svPosition;
		
		OUT.texCoord = IN[i].texCoord;
		OUT.position = worldPos.xyz;
		OUT.svPosition = mul(viewProjMatrix, worldPos);

		stream.Append(OUT);
	}
	
    stream.RestartStrip();
}