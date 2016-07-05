#pragma once

#ifndef DX12_ENGINE_MVP_H
#define DX12_ENGINE_MVP_H

#include "math.h"

using namespace DirectX;
using namespace SimpleMath;

#define DEFAULT_CAMERA_MOVE_SPEED		.005
#define DEFAULT_CAMERA_ROTATION_SPEED	.01

namespace DX12Engine {

	class Camera {
	private:
		Matrix	mDefaultModelMatrix;
		Matrix	mViewMatrix;
		Matrix	mProjectionMatrix;

		Vector4	mCameraPosition;
		Vector4	mCameraAt;
		Vector4	mCameraUp;

		float	mCameraHorizontalAngle;
		float	mCameraVerticalAngle;

		float	mCameraMoveSpeed;
		float	mCameraRotateSpeed;

		float	mCameraLocked;
		float	mDelta;

	public:

		Camera();
		~Camera();
		
		void Update(float delta);

		void moveForwards();
		void moveBackwards();
		void moveLeft();
		void moveRight();

		void rotateHorizontal(int deltaX);
		void rotateVertical(int deltaY);

		void calculateTarget();
		void calculateViewMatrix();

		void ViewMatrix(XMFLOAT4X4 newViewMatrix);
		Matrix ViewMatrix();
		void ProjectionMatrix(XMFLOAT4X4 newProjectionMatrix);
		Matrix ProjectionMatrix();

	};

}
#endif