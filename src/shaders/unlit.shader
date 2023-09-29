#include "RGBHSL.hlsli"

struct VIn
{
    float4 position : POSITION;
    float4 color : COLOR;
};
struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float fTime;
};

VOut VShader(VIn input)
{
    VOut output;
    

    output.position = input.position;
    //output.color = HSLtoRGB(float4(sin(fTime/10 + color.x)*0.5 + 0.5, 1, 0.5, 1));
    output.color = input.color;
    
    output.color.z = abs(frac(fTime / 25) * 2 - 1);
    
    return output;
}


float4 PShader(VOut input) : SV_TARGET
{

    return float4(input.position.x / 200, input.position.y / 150, input.color.z, 1);
}