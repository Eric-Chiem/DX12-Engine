#pragma once

#ifndef DX12_ENGINE_MVP_H
#define DX12_ENGINE_MVP_H

#include "math.h"

using namespace DirectX;
using namespace SimpleMath;

#define DEFAULT_CAMERA_MOVE_SPEED		5;
#define DEFAULT_CAMERA_ROTATION_SPEED	1;

namespace DX12Engine{

class Camera {
private:
	Matrix	mDefaultModelMatrix;
	Matrix	mViewMatrix;
	Matrix	mProjectionMatrix;

	Vector4	mCameraPosition;
	Vector4	mCameraTarget;
	Vector4	mCameraUp;

	float	mCameraHorizontalAngle;
	float	mCameraVerticalAngle;

	float	mCameraMoveSpeed;
	float	mCameraRotateSpeed;

	float	mCameraLocked;

public:

	Camera();
	~Camera();

	void Update();

	void calculateTarget();
	void calculateViewMatrix();

	void ViewMatrix(XMFLOAT4X4 newViewMatrix);
	Matrix ViewMatrix();
	void ProjectionMatrix(XMFLOAT4X4 newProjectionMatrix);
	Matrix ProjectionMatrix();

};

}
#endif