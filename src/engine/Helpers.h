#pragma once
#include <DirectXMath.h>

namespace Lapis
{
	namespace Helpers
	{
		constexpr Lapis::Vec3 AxisY = Lapis::Vec3(0.0, 1.0, 0.0);
		


		DirectX::XMMATRIX XMMatrixScaling(Lapis::Vec3 scale) {

			return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		}

		DirectX::XMMATRIX XMMatrixRotationRollPitchYaw(Lapis::Vec3 rot) {
			return DirectX::XMMatrixRotationRollPitchYaw(rot.z, rot.x, rot.y);
		}

		DirectX::XMMATRIX XMMatrixRotationAxis(Lapis::Vec3 axis, float angle) {
			return DirectX::XMMatrixRotationAxis({axis.x, axis.y, axis.z}, angle);
		}

		DirectX::XMMATRIX XMMatrixTranslation(Lapis::Vec3 pos) {

			return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		}

		DirectX::XMVECTOR XMVectorSet(Lapis::Vec4 vec)
		{
			return DirectX::XMVectorSet(vec.x, vec.y, vec.z, vec.w);
		}

	}
}