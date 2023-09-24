#pragma once

#include <windows.h>
#include <windowsx.h>
#include <D3D11.h>

struct VERTEX
{
	FLOAT x, y, z;
	DXGI_RGBA color;
};

class Rect2D
{
	int x, y, z, w;

};

struct LapisCommand
{
	UINT VertexCount;
	UINT StartVertexLocation;
	D3D_PRIMITIVE_TOPOLOGY TopologyType;
};

_declspec(align(16))
struct VS_CONSTANT_BUFFER
{
	float fTime;
};