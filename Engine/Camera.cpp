#include "Camera.h"

using namespace DX12Engine;

Camera::Camera() :
	mCameraLocked(false),
	mCameraMoveSpeed(5),
	mCameraRotateSpeed(1),
	mDefaultModelMatrix(XMMatrixIdentity()),
	mCameraPosition(0.0f, 2.0f, -4.0f, 0.0f),
	mCameraHorizontalAngle(0),
	mCameraVerticalAngle(0),
	mProjectionMatrix(XMMatrixPerspectiveFovLH(45.0f*(3.14f / 180.0f), (float)16 / (float)9, 0.1f, 1000.0f))
	{
		calculateViewMatrix;
}

Camera::~Camera() {


}

void Camera::Update() {
	if (!mCameraLocked) {
		calculateViewMatrix;
	}
	else {
	
	}
}

void Camera::calculateTarget() {
	mCameraTarget = XMFLOAT4(	cos(mCameraHorizontalAngle) * cos(mCameraVerticalAngle),
								sin(mCameraHorizontalAngle) * cos(mCameraVerticalAngle),
								sin(mCameraVerticalAngle),
								0.0f);
	mCameraUp = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
}

void Camera::calculateViewMatrix(){
	mViewMatrix = XMMatrixLookAtLH(mCameraPosition, mCameraTarget, mCameraUp);
}

void Camera::ViewMatrix(XMFLOAT4X4 newViewMatrix) {
	mViewMatrix = newViewMatrix;
}

Matrix Camera::ViewMatrix() {
	return mViewMatrix;
}

void Camera::ProjectionMatrix(XMFLOAT4X4 newProjectionMatrix) {
	mProjectionMatrix = newProjectionMatrix;
}

Matrix Camera::ProjectionMatrix() {
	return mProjectionMatrix;
}