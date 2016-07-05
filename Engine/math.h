#pragma once

#ifndef DX12_ENGINE_MATH_H
#define DX12_ENGINE_MATH_H

#include <SimpleMath.h>

using namespace DirectX;
using namespace SimpleMath;

#define PI 3.14159265


namespace DX12Engine {
	
	Vector4 pointAt(float horizontalAngle, float verticalAngle);

}

#endif