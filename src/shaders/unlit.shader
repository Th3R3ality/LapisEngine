#include "Header.hlsli"

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float fTime;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;
    

    output.position = position;
    //output.color = HSLtoRGB(float4(sin(fTime/10 + color.x)*0.5 + 0.5, 1, 0.5, 1));
    output.color = color;
    
    output.color.z = abs(frac(fTime / 25) * 2 - 1);
    
    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{

    return float4(position.x / 200, position.y / 150, color.z, 1);
}