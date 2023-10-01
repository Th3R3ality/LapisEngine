#include "RGBHSL.hlsli"

struct VIn
{
    float4 position : POSITION0;
    float4 color : COLOR0;
    float4 texCoord : TEXCOORD0;
};
struct VOut
{
    float4 position : SV_Position;
    float4 color : COLOR0;
    float4 texCoord : TEXCOORD0;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 mvp;
    float fTime;
};

float r = 200;
float l = 0;
float t = 0;
float b = 150;
/*
matrix<float, 4, 4> mat = {
    2 / (r - l), 0.0f, 0.0f, 0.0f,
    0.0f, 2 / (t - b), 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    };
*/

VOut VShader(VIn input)
{
    VOut output;


    output.position = mul(mvp, input.position);
    output.color = input.color;
    output.texCoord = input.texCoord;
    
    
    
    output.color.z = abs(frac(fTime / 25) * 2 - 1);
    
    return output;
}


float4 PShader(VOut input) : SV_TARGET
{
    float4 col = float4(1.0, 1.0, 1.0, 1.0);
    col.xy = input.texCoord;
    col.z = input.color.z;
    return col;
    //return float4(input.position.x / 200, input.position.y / 150, input.color.z, 1);
}