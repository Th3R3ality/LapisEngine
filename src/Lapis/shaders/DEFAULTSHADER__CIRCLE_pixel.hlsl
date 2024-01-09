#include "DefaultStructs.hlsli"

float circle_sdf(float2 p)
{
    p += -0.5;
    p *= 2;
                
    
    return 1-length(p);
}


float4 main(VOut input) : SV_TARGET
{
    float4 col = input.color;

    float alpha = circle_sdf(input.uv.xy);
    col.a = smoothstep(0, 0.03, alpha);
    clip(alpha);
    
    return col;
}