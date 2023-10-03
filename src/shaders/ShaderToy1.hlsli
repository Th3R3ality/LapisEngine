
float3 palette(float t)
{
    float3 a = float3(0.5, 0.5, 0.5);
    float3 b = float3(0.5, 0.5, 0.5);
    float3 c = float3(1.0, 1.0, 1.0);
    float3 d = float3(0.263, 0.416, 0.557);

    return a + b * cos(6.28318 * (c * t + d));
}

float4 ShaderToy1(float fTime, float2 coord, bool screenspace)
{
    float time = fTime / 10;
 
    float2 res = float2(200, 150);
    float2 uv = float2(0.0, 0.0);
    if (screenspace)
    {
        uv = (coord * 2.0 - res.xy) / res.y;
    }
    else
    {
        uv = coord - 0.5;
    }
    float2 uv0 = uv;
    float3 finalColor = float3(0, 0, 0);
    
    for (float i = 0.0; i < 4.0; i++)
    {
        uv = frac(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        float3 col = palette(length(uv0) + i * 0.4 + time * 0.4);

        d = sin(d * 8.0 + time) / 8.0;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }
        
    float4 fragColor = float4(finalColor.xyz, 1.0);
    
    return fragColor;
}