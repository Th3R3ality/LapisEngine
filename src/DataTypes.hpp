#pragma once

#include <windows.h>
#include <windowsx.h>
#include <D3D11.h>
#include <DirectXMath.h>

struct VERTEX
{
	FLOAT x, y, z;
	DXGI_RGBA color;
	DirectX::XMFLOAT4 texCoord;
};

struct LapisCommand
{
	UINT VertexCount;
	UINT StartVertexLocation;
	D3D_PRIMITIVE_TOPOLOGY TopologyType;
};

_declspec(align(16))
struct CONSTANT_BUFFER
{
	float mvp[4][4];
	float fTime;
};