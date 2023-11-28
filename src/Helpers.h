#pragma once
#include <DirectXMath.h>

namespace Lapis
{
	namespace Helpers
	{
		constexpr Lapis::Vector3 AxisY = Lapis::Vector3(0.0, 1.0, 0.0);
		


		DirectX::XMMATRIX XMMatrixScaling(Lapis::Vector3 scale) {

			return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		}

		DirectX::XMMATRIX XMMatrixRotationRollPitchYaw(Lapis::Vector3 rot) {
			return DirectX::XMMatrixRotationRollPitchYaw(rot.z, rot.x, rot.y);
		}

		DirectX::XMMATRIX XMMatrixRotationAxis(Lapis::Vector3 axis, float angle) {
			return DirectX::XMMatrixRotationAxis({axis.x, axis.y, axis.z}, angle);
		}

		DirectX::XMMATRIX XMMatrixTranslation(Lapis::Vector3 pos) {

			return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		}

		DirectX::XMVECTOR XMVectorSet(Lapis::Vector4 vec)
		{
			return DirectX::XMVectorSet(vec.x, vec.y, vec.z, vec.w);
		}

	}
}