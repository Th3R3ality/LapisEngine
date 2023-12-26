#include "DefaultStructs.hlsli"

VOut main(VIn input)
{
    VOut output;

    output.position = input.position;
    output.color = input.color;
    output.uv = input.uv;
    output.normal = input.normal;

    output.position = mul(output.position, Model);
    output.position = mul(output.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);

    return output;
}