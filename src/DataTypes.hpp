#pragma once

#include <windows.h>
#include <windowsx.h>
#include <D3D11.h>
#include <DirectXMath.h>

namespace Lapis
{
	struct Vector2
	{
		float x, y;
	};

	struct Vector3
	{
		float x, y, z;

		Vector3 operator-();
	};

	struct Vector4
	{
		float x, y, z, w;

		Vector4 operator+(const Vector4& other);
	};

	struct VERTEX
	{
		Vector3 pos;
		DXGI_RGBA color;
		DirectX::XMFLOAT4 texCoord;
	};

	struct Transform {
		Vector3 pos;
		Vector3 rot;
		Vector3 scale;
	};

	struct LapisCommand
	{
		UINT VertexCount;
		UINT StartVertexLocation;
		D3D_PRIMITIVE_TOPOLOGY TopologyType;
	};

	_declspec(align(16))
	struct GlobalConstantBuffer
	{
		float fTime;
		DirectX::XMMATRIX Screen;
		DirectX::XMMATRIX Model;
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};


}