
#define MIN_VIEW 0.99994
#define MAX_VIEW 0.99996

struct VertexOut {
	float2 texCoord : TEXCOORD;
	float4 svPosition : SV_POSITION;
};

cbuffer Window : register(b0) {
	float4 windowSize;
};

SamplerState texSampler : register(s0);

Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D positionTexture : register(t2);
Texture2DMS<float, 4> depthTexture : register(t3);
Texture2D ssaoTexture : register(t4);
Texture2D skydomeTexture : register(t5);

float4 main(VertexOut IN) : SV_TARGET {
	float4 OUT;

	// Sample Textures
	float3 color = colorTexture.Sample(texSampler, IN.texCoord);
	float3 normal = normalTexture.Sample(texSampler, IN.texCoord);
	float3 position = positionTexture.Sample(texSampler, IN.texCoord);
	float3 ssao = ssaoTexture.Sample(texSampler, IN.texCoord);
	float3 skydome = skydomeTexture.Sample(texSampler, IN.texCoord);

	// Depth
	float depth = 0;
	for (int i = 0; i < 4; ++i) {
		depth += depthTexture.Load(IN.texCoord * windowSize.xy, i).r;
	}
	depth /= 4.0;
		
	// Normalize Textures
	normal = normalize(normal * 2.0 - 1.0);
	
	// Calculate Lighting
	float3 lightDir = normalize(float3(1, 1, 1));
	float cosTheta = max(dot(normal, lightDir), 0.1);
	
	OUT = float4(color * ssao * cosTheta, 1.0);

	// Poor-man's stencil
	if(depth > MIN_VIEW) {
		OUT = lerp(OUT, float4(skydome, 1.0), clamp((depth - MIN_VIEW) / (1.0 - MAX_VIEW), 0, 1));
	}

	return OUT;
}