
struct DomainOut {
	float2 texCoord : TEXCOORD0;
	float4 svPosition : SV_POSITION;
};

struct GeometryOut {
	float2 texCoord : TEXCOORD;
	float4 viewSpacePos : VIEWSPACE;
	float3 position : POSITION;
	float3 tangent : TANGENT;
	float4 svPosition : SV_POSITION;
	//float svClip[6] : SV_ClipDistance0;
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

SamplerState texSampler : register(s0);

Texture2D material0Disp : register(t3);
Texture2D material0Alpha : register(t5);

Texture2D material1Disp : register(t7);
Texture2D material1Alpha : register(t9);

Texture2D material2Disp : register(t11);
Texture2D material2Alpha : register(t13);

Texture2D material3Disp : register(t15);
Texture2D material3Alpha : register(t17);

[maxvertexcount(3)]
void main(
	triangle DomainOut IN[3],
	inout TriangleStream<GeometryOut> stream) {
	
	float3 tangent = float3(0, 0, 0);
	float3 displacement[3] = { float3(0, 0, 0), float3(0, 0, 0), float3(0, 0, 0) };
	
	float dist = 
		distance(IN[0].svPosition.xyz, camPosition.xyz) + 
		distance(IN[1].svPosition.xyz, camPosition.xyz) + 
		distance(IN[2].svPosition.xyz, camPosition.xyz);
	dist /= 3.0;

	if (dist < detailRangeFar) {
		
		float material0Scale = 4.0;
		float material1Scale = 4.0;
		float material2Scale = 10.0;
		float material3Scale = 10.0;

		float material0Height = 0.0;
		float material1Height = 1.0;
		float material2Height = 3.0;
		float material3Height = 3.0;
		
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
	
		tangent = normalize((e1 * deltaUV2.y - e2 * deltaUV1.y)*r);

		for (int i = 0; i < 3; ++i) {
			
			float2 mapCoords = (IN[i].svPosition.xz + scaleXZ / 2) / scaleXZ;

			displacement[i] = float3(0, 1, 0);

			float height = IN[i].svPosition.y;

			float scale = 0;
			scale += material0Disp.SampleLevel(texSampler, IN[i].texCoord / material0Scale, 0).r * material0Height * material0Alpha.SampleLevel(texSampler, mapCoords, 0).r;
			scale += material1Disp.SampleLevel(texSampler, IN[i].texCoord / material1Scale, 0).r * material1Height * material1Alpha.SampleLevel(texSampler, mapCoords, 0).r;
			scale += material2Disp.SampleLevel(texSampler, IN[i].texCoord / material2Scale, 0).r * material2Height * material2Alpha.SampleLevel(texSampler, mapCoords, 0).r;
			scale += material3Disp.SampleLevel(texSampler, IN[i].texCoord / material3Scale, 0).r * material3Height * material3Alpha.SampleLevel(texSampler, mapCoords, 0).r;

			float attenuation = clamp(-distance(IN[i].svPosition.xyz, camPosition.xyz) / detailRangeNear + 1, 0, 1);
			scale *= attenuation;

			displacement[i] *= scale;
		}
	}
	
	for (int i = 0; i < 3; ++i) {
		GeometryOut OUT;

		float4 worldPos = IN[i].svPosition + float4(displacement[i], 0);
		
		OUT.texCoord = IN[i].texCoord;
		OUT.viewSpacePos = mul(viewMatrix, worldPos);
		OUT.position = worldPos.xyz;
		OUT.tangent = tangent;
		
		float4 position = mul(viewProjMatrix, worldPos);
		/*		
		OUT.svClip[0] = dot(position, frustumPlanes[0]);
		OUT.svClip[1] = dot(position, frustumPlanes[1]);
		OUT.svClip[2] = dot(position, frustumPlanes[2]);
		OUT.svClip[3] = dot(position, frustumPlanes[3]);
		OUT.svClip[4] = dot(position, frustumPlanes[4]);
		OUT.svClip[5] = dot(position, frustumPlanes[5]);
		*/

		OUT.svPosition = position;

		stream.Append(OUT);
	}
	
    stream.RestartStrip();
}