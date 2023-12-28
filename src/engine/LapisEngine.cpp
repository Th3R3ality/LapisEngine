#include "LapisEngine.h"
#include "Backend.h"

namespace Lapis
{
	void Init(RenderApi renderingApi, HWND hwnd)
	{
		switch (renderingApi) {
		case RenderApi::D3D11:
			Backend::InitD3D11(hwnd);
		}
	}

}
