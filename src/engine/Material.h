#pragma once
#include <d3d11.h>
#include "Helpers.h"

#include <iostream>

namespace Lapis {


	class Material {
	public:
		Material() :
			vertexShader(nullptr), pixelShader(nullptr), blendState(nullptr)
		{}

		Material(ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11BlendState* bs) :
			vertexShader(vs), pixelShader(ps), blendState(bs)
		{}

		~Material()
		{
			safe_release(vertexShader);
			safe_release(pixelShader);
			safe_release(blendState);
		}

		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11BlendState* blendState;
	};
}