#pragma once

#ifndef DX12_ENGINE_VERTEX_H
#define DX12_ENGINE_VERTEX_H

#include "math.h"

namespace DX12Engine {

	struct Vertex {
		Vertex(float x, float y, float z, float u, float v) : pos(x, y, z), texCoord(u, v) {}
		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
	};

}

#endif