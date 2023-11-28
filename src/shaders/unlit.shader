#include "RGBHSL.hlsli"

#include "ShaderToy1.hlsli"

#include "ShaderToy2.hlsli"

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

cbuffer GlobalConstantBuffer : register(b0)
{
    float fTime;
    matrix Screen;
    matrix Model;
    matrix World;
    matrix View;
    matrix Projection;
}

VOut VShader(VIn input)
{
    VOut output;


    //output.position = mul(Screen, input.position);
    output.color = input.color;
    output.texCoord = input.texCoord;
    output.position = input.position;
    
    output.position = mul(output.position, Model);
    output.position = mul(output.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);

    return output;
}

float4 PShader(VOut input) : SV_TARGET
{
    //return ShaderToy1(fTime, input.position.xy, 1);
    
    //return ShaderToy2(input.position.xy, fTime);
    
    return input.color;
    return input.texCoord;
    return input.texCoord * input.color;
    
    float4 col = float4(0.0, 0.0, 0.0, 1.0);
    col.xy = input.texCoord;
    //col.z = input.color.z;
    return col;
}