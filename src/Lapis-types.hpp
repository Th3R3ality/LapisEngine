#pragma once

#include <windows.h>
#include <windowsx.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

class worldObject {
private:
	int x, y, z;
	D3D10_PRIMITIVE_TOPOLOGY topology;

public:
	worldObject()
		: x(0), y(0), z(0),
		topology(D3D10_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST) {};

	worldObject(int x, int y, int z, D3D10_PRIMITIVE_TOPOLOGY topology)
		: x(x), y(y), z(z),
		topology(topology) {};
};

class screenObject {
private:
	int x, y;
	D3D10_PRIMITIVE_TOPOLOGY topology;

public:
	screenObject()
		: x(0), y(0),
		topology(D3D10_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST) {};

	screenObject(int x, int y, D3D10_PRIMITIVE_TOPOLOGY topology)
		: x(x), y(y),
		topology(topology) {};
};