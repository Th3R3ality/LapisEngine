#include "DefaultStructs.hlsli"

VOut main( VIn input )
{
    VOut output;
    
    output.position = input.position;
    output.color = input.color;
    output.uv = input.uv;
    output.normal = input.normal;
    
    
    output.position = mul(Screen, output.position);
    
	return output;
}