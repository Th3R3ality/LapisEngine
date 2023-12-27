#pragma once
#include <d3d11.h>

class rgb
{
public:
	unsigned char R;
	unsigned char G;
	unsigned char B;

	rgb(unsigned char r, unsigned char g, unsigned char b);
	

	bool Equals(rgb rgb);

};

class hsl
{
public:
	int H;
	float S;
	float L;

	hsl(int h, float s, float l);
	bool Equals(hsl hsl);
};

static float HueToRGB(float v1, float v2, float vH);
static rgb hsl2rgb(hsl hsl);
DXGI_RGBA hsl2rgb(int h, float s, float l, float a);
