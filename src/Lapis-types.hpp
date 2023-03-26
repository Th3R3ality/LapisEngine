#pragma once

#include <windows.h>
#include <windowsx.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

struct VERTEX
{
	FLOAT x, y, z;
	D3DXCOLOR color;
};

class square
{
private:
	int x, y, w, h;
	D3DXCOLOR color;

public:

	square() : x(0), y(0), w(0), h(0), color(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f))  {};
	square(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {};

};
