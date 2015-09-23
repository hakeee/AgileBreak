

Texture2D tex : register(t0);

struct PSIn {
	float4 screenPos : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};


SamplerState MeshTextureSampler : register(s0);

float4 main(PSIn input) : SV_TARGET
{
	return tex.Sample(MeshTextureSampler, input.texCoords);
}