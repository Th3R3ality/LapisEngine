float2 iResolution = float2(200, 150);
float iTime = 0;
float opSmoothUnion(float d1, float d2, float k)
{
    float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return lerp(d2, d1, h) - k * h * (1.0 - h);
}
float sdSphere(float3 p, float s)
{
    return length(p) - s;
}

float map(float3 p)
{
    float d = 2.0;
    for (int i = 0; i < 16; i++)
    {
        float fi = float(i);
        float time = iTime * (frac(fi * 412.531 + 0.513) - 0.5) * 2.0;
        float fix = lerp(0.5, 1.0, frac(fi * 412.531 + 0.5124));
        d = opSmoothUnion(
            sdSphere(p + sin(time + fi * float3(52.5126, 64.62744, 632.25)) * float3(2.0, 2.0, 0.8), fix),
			d,
			0.4
		);
    }
    return d;
}

float3 calcNormal(float3 p)
{
    const float h = 1e-5; // or some other value
    const float2 k = float2(1, -1);
    float fix2 = k.xyy * map(p + k.xyy * h) +
                      k.yyx * map(p + k.yyx * h) +
                      k.yxy * map(p + k.yxy * h) +
                      k.xxx * map(p + k.xxx * h);
    return normalize(float3(fix2, fix2, fix2));
}

float4 ShaderToy2(float2 fragCoord, float time)
{
    float2 resxy = iResolution.xy * saturate(time / 100);
    float2 uv = fragCoord / 3;
    //uv = fragCoord;
    float3 rayOri = float3((uv - 0.5) * float2(iResolution.x / iResolution.y, 1.0) * 6.0, 3.0);
    float3 rayDir = float3(0.0, 0.0, -1.0);
	
    float depth = 0.0;
    float3 p = float3(0, 0, 0);
	
    for (int i = 0; i < 64; i++)
    {
        p = rayOri + rayDir * depth;
        float dist = map(p);
        depth += dist;
        if (dist < 1e-6)
        {
            break;
        }
    }
	
    depth = min(6.0, depth);
    float3 n = calcNormal(p);
    float b = max(0.0, dot(n, float3(0.577, 0.577, 0.577)));
    float3 col = (0.5 + 0.5 * cos((b + iTime * 3.0) + uv.xyx * 2.0 + float3(0, 2, 4))) * (0.85 + b * 0.35);
    col *= exp(-depth * 0.15);
	
    // maximum thickness is 2m in alpha channel
    return float4(col, 1.0 - (depth - 0.5) / 2.0);
}

//*/
