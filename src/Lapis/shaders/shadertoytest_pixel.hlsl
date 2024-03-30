#include "DefaultStructs.hlsli"
#include "ShaderToy1.hlsli"

float4 main(VOut input) : SV_TARGET
{
    return ShaderToy1(elapsedTime, input.position.xy, 1);
}