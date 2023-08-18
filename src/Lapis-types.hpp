#pragma once

#include <windows.h>
#include <windowsx.h>
#include <D3D11.h>

struct VERTEX
{
	FLOAT x, y, z;
	DXGI_RGBA color;
};

class square
{
private:
	int x, y, w, h;
	DXGI_RGBA color;

public:

	square() : x(0), y(0), w(0), h(0), color(DXGI_RGBA(1.0f, 0.0f, 1.0f, 1.0f))  {};
	square(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {};

};
