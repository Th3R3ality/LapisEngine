#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <string>

#include "LapisTypes.h"
#include "Helpers.h"

namespace Lapis
{
	using Topology = D3D_PRIMITIVE_TOPOLOGY;

	struct LapisCommand
	{
		UINT vertexCount;
		Topology topology;
		Transform transform;
		std::string materialName;
	};

	_declspec(align(16))
		struct GlobalConstantBuffer
	{
		float elapsedTime;
		float deltaTime;
		DirectX::XMMATRIX Screen;
		DirectX::XMMATRIX Model;
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};

	class InternalMaterial
	{
	public:
		std::string name;
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11BlendState* blendState;
		

		InternalMaterial() :
			name("UNNAMED_MATERIAL"), vertexShader(nullptr), pixelShader(nullptr), blendState(nullptr)
		{}

		InternalMaterial(std::string name, ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11BlendState* bs) :
			name(name), vertexShader(vs), pixelShader(ps), blendState(bs)
		{}

		~InternalMaterial()
		{
			safe_release(vertexShader);
			safe_release(pixelShader);
			safe_release(blendState);
		}
	};

	struct InternalLapisCommand
	{
		UINT vertexCount;
		UINT startVertexLocation;
		D3D_PRIMITIVE_TOPOLOGY topology;
		Transform transform;
		std::shared_ptr<InternalMaterial> material;

		InternalLapisCommand(LapisCommand lapisCommand, UINT startVertexLocation);
	};
}