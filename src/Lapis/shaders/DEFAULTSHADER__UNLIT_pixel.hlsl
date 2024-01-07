#include "DefaultStructs.hlsli"

float4 main(VOut input) : SV_TARGET
{
    //return float4(input.uv.xyz, 1);
    return input.color;
}