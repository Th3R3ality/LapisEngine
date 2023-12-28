#include "RGBHSL.hlsli"

#include "DefaultStructs.hlsli"

VOut VShader(VIn input)
{
    VOut output;


    output.color = input.color;
    output.uv = input.uv;
    output.position = input.position;
    
    output.position = mul(Screen, output.position);

    //output.position = mul(output.position, Model);
    //output.position = mul(output.position, World);
    //output.position = mul(output.position, View);
    //output.position = mul(output.position, Projection);

    return output;
}

float4 PShader(VOut input) : SV_TARGET
{
    //return ShaderToy1(fTime, input.position.xy, 1);
    
    //return ShaderToy2(input.position.xy, fTime);
    
    //return input.color;
    return input.uv;
    return input.uv * input.color;
    
    float4 col = float4(0.0, 0.0, 0.0, 1.0);
    col.xy = input.uv;
    //col.z = input.color.z;
    return col;
}