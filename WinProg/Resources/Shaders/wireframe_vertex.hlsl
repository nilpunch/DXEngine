cbuffer WVP : register(b0)
{
	float4x4 WVP;
};

struct VS_INPUT
{
	float3 inPos : POSITION;
	float4 inColor : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 coord : POSITION;
	float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = mul(float4(input.inPos, 1.0f), WVP);
	output.coord = input.inPos.xyz;
	output.color = input.inColor;
	return output;
}