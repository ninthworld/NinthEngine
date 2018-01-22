
struct GeometryOut {
	float2 texCoord : TEXCOORD;
	float3 position : POSITION;
	float3 tangent : TANGENT;
	float4 svPosition : SV_POSITION;
};
struct PixelOut {
	float4 color : SV_TARGET;
	float4 normal : SV_TARGET1;
	float4 position : SV_TARGET2;
};

cbuffer Camera : register(b0) {
	float4x4 camViewProj;
	float4 camPosition;
};

cbuffer Terrain : register(b1) {
	float4 terrainScale;
};

SamplerState texSampler : register(s0);

Texture2D normalmap : register(t1);
Texture2D splatmap : register(t2);

// Grass
Texture2D material0_color : register(t3);
Texture2D material0_normal : register(t4);

// Dirt 1
Texture2D material1_color : register(t5);
Texture2D material1_normal : register(t6);

// Dirt 2
Texture2D material2_color : register(t7);
Texture2D material2_normal : register(t8);

// Stone 1
Texture2D material3_color : register(t9);
Texture2D material3_normal : register(t10);

// Stone 2
Texture2D material4_color : register(t11);
Texture2D material4_normal : register(t12);

float getMap(float slope, float4 phase) {
	if (slope >= phase.x && slope < phase.y) {
		float d = phase.y - phase.x;
		if (d == 0) return 1.0;
		return (slope - phase.x) / d;
	}
	else if (slope >= phase.y && slope < phase.z) {
		return 1.0;
	}
	else if (slope >= phase.z && slope < phase.w) {
		float d = phase.w - phase.z;
		if (d == 0) return 1.0;
		return 1.0 - (slope - phase.z) / d;
	}
	else {
		return 0;
	}
}

PixelOut main(GeometryOut IN) {
	PixelOut OUT;

	float2 mapPosition = (IN.position.xz + terrainScale.x / 2.0) / terrainScale.x;

	float3 normal = normalmap.Sample(texSampler, mapPosition).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	
	float3 grassColor = material0_color.Sample(texSampler, IN.texCoord).rgb;
	float3 grassNormal = material0_normal.Sample(texSampler, IN.texCoord).rgb;

	float3 dirt1Color = material1_color.Sample(texSampler, IN.texCoord).rgb;
	float3 dirt1Normal = material1_normal.Sample(texSampler, IN.texCoord).rgb;

	float3 dirt2Color = material2_color.Sample(texSampler, IN.texCoord).rgb;
	float3 dirt2Normal = material2_normal.Sample(texSampler, IN.texCoord).rgb;

	float3 stone1Color = material3_color.Sample(texSampler, IN.texCoord / 2).rgb;
	float3 stone1Normal = material3_normal.Sample(texSampler, IN.texCoord / 2).rgb;
	
	float3 stone2Color = material4_color.Sample(texSampler, IN.texCoord / 2).rgb;
	float3 stone2Normal = material4_normal.Sample(texSampler, IN.texCoord / 2).rgb;
	
	float splat = 1.0;
	
	float3 dirtColor = lerp(dirt1Color, dirt2Color, splat);
	float3 dirtNormal = lerp(dirt1Normal, dirt2Normal, splat);

	float3 stoneColor = lerp(stone1Color, stone2Color, splat) * 0.6;
	float3 stoneNormal = lerp(stone1Normal, stone2Normal, splat);
	
	float slope = 1.0 - normal.z;
	
	float4 grassPhase = float4(0.0, 0.0, 0.2, 0.25);
	float4 dirtPhase = float4(0.2, 0.25, 0.3, 0.35);
	float4 stonePhase = float4(0.3, 0.35, 1.0, 1.0);

	float grassMap = getMap(slope, grassPhase);
	float dirtMap = getMap(slope, dirtPhase);
	float stoneMap = getMap(slope, stonePhase);
	
	float dist = length(camPosition.xyz - IN.position);
	if (dist < 400) {
		float attenuation = clamp(-dist / 400 + 1, 0, 1);

		float3 bitangent = normalize(cross(IN.tangent, normal));
		float3x3 TBN = float3x3(IN.tangent, bitangent, normal);

		float3 bumpNormal = float3(0, 0, 0);
		bumpNormal += (grassNormal * 2.0 - 1.0) * grassMap;
		bumpNormal += (dirtNormal * 2.0 - 1.0) * dirtMap;
		bumpNormal += (stoneNormal * 2.0 - 1.0) * stoneMap;
		bumpNormal = normalize(bumpNormal);

		bumpNormal.xy *= attenuation;

		normal = normalize(mul(TBN, bumpNormal));
	}
	
	OUT.position = float4((IN.position / 1000.0 + 1.0) / 2.0, 1.0); 
	OUT.normal = float4((normal + 1.0) / 2.0, 1.0);
	OUT.color = float4(
		grassColor * grassMap +
		dirtColor * dirtMap +
		stoneColor * stoneMap, 1.0);

	return OUT;
}