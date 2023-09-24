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
    output.color = mul(sin(fTime/10)*0.5 + 0.5, color);

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}