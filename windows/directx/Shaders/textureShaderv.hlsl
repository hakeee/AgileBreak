

struct VSIn {
	float3 position : POSITION;
	float2 texCoord : TEXCOORDS;
	float3 normal : NORMAL;
};

struct PSIn {
	float4 screenPos : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};

cbuffer Projection : register(b1) {
	float4x4 p;
}
cbuffer View : register(b2) {
	float4x4 v;
}
cbuffer Model : register(b3) {
	float4x4 m;
}





PSIn main( VSIn input )
{
	PSIn output;
	output.screenPos = mul(float4(input.position, 1.0), m);
	output.screenPos = mul(output.screenPos, v);
	output.screenPos = mul(output.screenPos, p);
	output.texCoords = input.texCoord;
	return output;
}