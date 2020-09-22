struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 coord : POSITION;
	float4 color : COLOR;
};


float4 main(PS_INPUT input) : SV_TARGET
{
	float3 uv = float3(input.coord.x / 2.0f + .5f, input.coord.y / 2.0f + .5f, input.coord.z / 2.0f + .5f);
	float3 color = 0.0f;
	float threshold = 0.9f / 2.0f;
	float t = 2.5f;
	
	if (abs(uv.x - .5f) > threshold && abs(uv.y - .5f) > threshold)
	{
		color.xyz = 1.0f;
		//color.z = step(uv.z, 0.5f);
		color.z = abs((uv.z - 0.5f) * t);
	}
	else if (abs(uv.x - .5f) > threshold && abs(uv.z - .5f) > threshold)
	{
		color.xyz = 1.0f;
		//color.y = step(uv.y, 0.5f);
		color.y = abs((uv.y - 0.5f) * t);
	}
	else if (abs(uv.y - .5f) > threshold && abs(uv.z - .5f) > threshold)
	{
		color.xyz = 1.0f;
		//color.x = step(uv.x, 0.5f);
		color.x = abs((uv.x - 0.5f) * t);
	}
	else
	{
		color.xyz = input.color.xyz;
	}
	
	return float4(color, 1.0f);
}