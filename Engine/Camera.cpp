#include "Camera.h"

using namespace DX12Engine;
using namespace DirectX;
using namespace SimpleMath;
Camera::Camera() :
	mCameraLocked(false),
	mCameraMoveSpeed(DEFAULT_CAMERA_MOVE_SPEED),
	mCameraRotateSpeed(DEFAULT_CAMERA_ROTATION_SPEED),
	mDefaultModelMatrix(XMMatrixIdentity()),
	mCameraPosition(0.0f, 0.0f, 0.0f, 0.0f),
	mCameraHorizontalAngle(0),
	mCameraVerticalAngle(0),
	mProjectionMatrix(XMMatrixPerspectiveFovLH(45.0f*(3.14f / 180.0f), (float)16 / (float)9, 0.1f, 1000.0f))
{
	calculateTarget();
	calculateViewMatrix();
}

Camera::~Camera() {


}

void Camera::Update(float delta){
	mDelta = delta;
	if (!mCameraLocked) {
		calculateTarget();
		calculateViewMatrix();
	}
	else {

	}
}

void Camera::moveForwards() {
	mCameraPosition += mCameraMoveSpeed * mDelta * mCameraAt;
}

void Camera::moveBackwards() {
	mCameraPosition -= mCameraMoveSpeed * mDelta * mCameraAt;
}

void Camera::moveLeft() {
	mCameraPosition += mCameraMoveSpeed * mDelta * pointAt(mCameraHorizontalAngle + PI / 2, mCameraVerticalAngle);
}

void Camera::moveRight() {
	mCameraPosition -= mCameraMoveSpeed * mDelta * pointAt(mCameraHorizontalAngle + PI / 2, mCameraVerticalAngle);
}

void Camera::rotateHorizontal(int deltaX) {
	mCameraHorizontalAngle += mCameraRotateSpeed * mDelta * deltaX;
}

void Camera::rotateVertical(int deltaY) {
	mCameraVerticalAngle += mCameraRotateSpeed * mDelta * deltaY;
	if (mCameraVerticalAngle > PI / 2) mCameraVerticalAngle = PI / 2;
	else if (mCameraVerticalAngle < -PI / 2) mCameraVerticalAngle = -PI / 2;
}

void Camera::calculateTarget() {
	mCameraAt = pointAt(mCameraHorizontalAngle, mCameraVerticalAngle);
	mCameraUp = pointAt(mCameraHorizontalAngle, mCameraVerticalAngle + PI / 2);
}

void Camera::calculateViewMatrix() {
	mViewMatrix = XMMatrixLookAtLH(mCameraPosition, mCameraAt + mCameraPosition, mCameraUp);
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