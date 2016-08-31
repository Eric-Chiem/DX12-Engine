#pragma once

#ifndef DX12_ENGINE_CONSTANT_BUFFER_PER_OBJECT_H
#define DX12_ENGINE_CONSTANT_BUFFER_PER_OBJECT_H

#include "math.h"

namespace DX12Engine {

	struct ConstantBufferPerObject {
		XMFLOAT4X4 wvpMat;
	};

}
#endif