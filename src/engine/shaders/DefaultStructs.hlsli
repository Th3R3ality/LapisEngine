struct VIn
{
    float4 position : POSITION;
    float4 color : COLOR;
    float4 uv : TEXCOORD0;
    float4 normal : NORMAL;
};
struct VOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float4 uv : TEXCOORD0;
    float4 normal : NORMAL;
};
cbuffer GlobalConstantBuffer : register(b0)
{
    float elapsedTime;
    float deltaTime;
    matrix Screen;
    matrix Model;
    matrix World;
    matrix View;
    matrix Projection;
}