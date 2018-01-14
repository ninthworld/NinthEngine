
struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

SamplerState texSampler : register(s0);

Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D positionTexture : register(t2);
Texture2D ssaoTexture : register(t3);

float4 main(VertexOut IN) : SV_TARGET {

	float3 color = colorTexture.Sample(texSampler, IN.texCoord);
	float3 normal = (normalTexture.Sample(texSampler, IN.texCoord) * 2.0 - 1.0);
	float3 position = positionTexture.Sample(texSampler, IN.texCoord);
	float3 ssao = ssaoTexture.Sample(texSampler, IN.texCoord);
		
	float3 lightDir = normalize(float3(1, 1, 1));
	float cosTheta = max(dot(normal, lightDir), 0.1);

	return float4(color * ssao * cosTheta, 1.0);
}