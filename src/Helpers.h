#pragma once
#include <DirectXMath.h>

namespace Lapis
{
	namespace Helpers
	{
		DirectX::XMVECTOR XMVectorSet(Lapis::Vector4 vec)
		{
			return DirectX::XMVectorSet(vec.x, vec.y, vec.z, vec.w);
		}
	}
}