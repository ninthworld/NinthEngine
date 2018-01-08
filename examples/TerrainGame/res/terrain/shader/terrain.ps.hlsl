
struct GeometryOut {
	float2 texCoord : TEXCOORD;
	float4 viewSpacePos : POSITION0;
	float3 position : POSITION1;
	float3 tangent : TANGENT;

	float4 svPosition : SV_POSITION;
	//float4 svClip : SV_ClipDistance;
};

struct PixelOut {
	float4 color : SV_TARGET;
};

cbuffer Camera : register(b0) {
	float4x4 viewMatrix;
	float4x4 viewProjMatrix;
	float4 camPosition;
	float4 frustumPlanes[6];
};

cbuffer Terrain : register(b1) {
	float4x4 worldMatrix;
	float scaleXZ;
	float scaleY;
	float detailRangeNear;
	float detailRangeFar;
	float lodRange[8];
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

SamplerState texSampler : register(s0);

Texture2D normalmap : register(t1);

Texture2D material0Dif : register(t2);
Texture2D material0Norm : register(t4);
Texture2D material0Alpha : register(t5);

Texture2D material1Dif : register(t6);
Texture2D material1Norm : register(t8);
Texture2D material1Alpha : register(t9);

Texture2D material2Dif : register(t10);
Texture2D material2Norm : register(t12);
Texture2D material2Alpha : register(t13);

Texture2D material3Dif : register(t14);
Texture2D material3Norm : register(t16);
Texture2D material3Alpha : register(t17);

PixelOut main(GeometryOut IN) {
	PixelOut OUT;

	float dist = length(camPosition.xyz - IN.position);
	float height = IN.position.y;

	float2 mapCoords = (IN.position.xz + scaleXZ / 2.0) / scaleXZ;

	float3 normal = normalize(normalmap.Sample(texSampler, mapCoords).rgb * 2.0 - 1.0);

	float material0Scale = 4.0;
	float material1Scale = 4.0;
	float material2Scale = 10.0;
	float material3Scale = 10.0;

	float alpha0 = material0Alpha.Sample(texSampler, mapCoords).r;
	float alpha1 = material1Alpha.Sample(texSampler, mapCoords).r;
	float alpha2 = material2Alpha.Sample(texSampler, mapCoords).r;
	float alpha3 = material3Alpha.Sample(texSampler, mapCoords).r;
	
	float3 color0 = material0Dif.Sample(texSampler, IN.texCoord / material0Scale).rgb;
	float3 color1 = material1Dif.Sample(texSampler, IN.texCoord / material1Scale).rgb;
	float3 color2 = material2Dif.Sample(texSampler, IN.texCoord / material2Scale).rgb;
	float3 color3 = material3Dif.Sample(texSampler, IN.texCoord / material3Scale).rgb;

	if (dist < detailRangeNear) {
		
		float attenuation = clamp(-dist / detailRangeNear + 1, 0, 1);

		float3 bitangent = normalize(cross(IN.tangent, normal));
		float3x3 TBN = float3x3(IN.tangent, bitangent, normal);

		float3 bumpNormal = float3(0);
		bumpNormal += (material0Norm.Sample(texSampler, IN.texCoord / material0Scale).rgb * 2.0 - 1.0) * alpha0;
		bumpNormal += (material1Norm.Sample(texSampler, IN.texCoord / material1Scale).rgb * 2.0 - 1.0) * alpha1;
		bumpNormal += (material2Norm.Sample(texSampler, IN.texCoord / material2Scale).rgb * 2.0 - 1.0) * alpha2;
		bumpNormal += (material3Norm.Sample(texSampler, IN.texCoord / material3Scale).rgb * 2.0 - 1.0) * alpha3;
		bumpNormal = normalize(bumpNormal);

		bumpNormal.xy *= attenuation;

		normal = normalize(mul(TBN, bumpNormal));
	}
	
	float3 lightDir = normalize(float3(1, 1, 1));

	float cosTheta = max(dot(normal, lightDir), 0.1);

	float3 fragColor = 
		color0 * alpha0 +
		color1 * alpha1 +
		color2 * alpha2 +
		color3 * alpha3;

	OUT.color = float4(fragColor * cosTheta, 1.0);

	return OUT;
}