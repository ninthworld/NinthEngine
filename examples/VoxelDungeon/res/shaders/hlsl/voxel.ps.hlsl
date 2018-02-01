
struct VertexOut {
	float4 material : TEXCOORD;
	float3 normal : NORMAL;
	float3 position : POSITION;
	float4 svPosition : SV_POSITION;
};

struct PixelOut {
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
};

SamplerState texSampler : register(s0);

Texture2D stoneFloor : register(t0);
Texture2D stoneWall : register(t1);

PixelOut main(VertexOut IN) {
	PixelOut OUT;

	float3 color = IN.material.rgb;

	if (IN.material.a == 1) {
		color = stoneFloor.Sample(texSampler, IN.position.xz / 32.0).rgb;
	}
	else if (IN.material.a == 2) {
		float2 texCoord = float2(0, 0);
		if (IN.normal.x > 0) texCoord = IN.position.xy / float2(64, -64);
		if (IN.normal.x < 0) texCoord = IN.position.xy / float2(64, -64);
		if (IN.normal.z > 0) texCoord = IN.position.zy / float2(64, -64);
		if (IN.normal.z < 0) texCoord = IN.position.zy / float2(64, -64);
		color = stoneWall.Sample(texSampler, texCoord).rgb;
	}

	OUT.color = float4(color, 1.0);
	OUT.normal = float4(IN.normal, 1.0);

	return OUT;
}