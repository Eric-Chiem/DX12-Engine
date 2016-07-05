#include "math.h"

namespace DX12Engine {

	Vector4 pointAt(float horizontalAngle, float verticalAngle) {
		
		return XMFLOAT4(cos(horizontalAngle) * cos(verticalAngle),
			sin(verticalAngle),
			sin(horizontalAngle) * cos(verticalAngle),
			0.0f);

	}

}