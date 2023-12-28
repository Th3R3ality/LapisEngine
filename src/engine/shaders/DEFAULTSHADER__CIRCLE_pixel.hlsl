#include "DefaultStructs.hlsli"

float rounded_box_sdf(float2 p, float2 dimensions, float rounding)
{
    p += -0.5;
    p *= 2;
                
    float2 d = abs(p) - dimensions * 0.5 + rounding;
    return 1 - length(max(d, 0)) - rounding;
}


float4 main(VOut input) : SV_TARGET
{
    //float f = (sin(elapsedTime / 2) + 1) / 4;

    float4 col = input.color;

    float rounding = 0.5;
    float dimension = 2;
    float2 dimensions = float2(dimension, dimension);
    rounding *= dimensions.x;

    float alpha = rounded_box_sdf(input.uv.xy, dimensions, rounding);

    col.a *= alpha >= 1 - rounding * 2 ? 1 : -1;

    clip(col.a);
    
    return col;
}