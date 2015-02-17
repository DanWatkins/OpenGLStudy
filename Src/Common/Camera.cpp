#include "Camera.h"

Camera::Camera() :
	mPosition(-20, 0, -20),
	mHorizontalAngle(0.0f),
	mVerticalAngle(0.0f),
	mFieldOfView(50.0f),
	mNearPlane(0.01f),
	mFarPlane(1500),
	mAspectRatio(1), 
	mMaxVerticalAngle(95.0f)
{
	lookAt(Vec3(60, 20, 60));
}


void Camera::resize(unsigned windowWidth, unsigned windowHeight)
{
	mAspectRatio = (float)windowWidth / (float)windowHeight;
}


Mat4 Camera::orientaion() const
{
	Mat4 orientation;
	orientation = glm::rotate(orientation, mVerticalAngle, Vec3(1, 0, 0));
	orientation = glm::rotate(orientation, mHorizontalAngle, Vec3(0, 1, 0));
	return orientation;
}


void Camera::incOrientation(float rightAngle, float upAngle)
{
	mHorizontalAngle += rightAngle;
	mVerticalAngle += upAngle;
	normalizeAngles();
}


void Camera::lookAt(Vec3 position)
{
	if (position == mPosition)
	{
		std::cout << "MEGA ERROR: You are trying to look at your origin" << std::endl;
		return;
	}

	Vec3 direction = glm::normalize(position - mPosition);
	mVerticalAngle = radToDeg(asinf(-direction.y));
	mHorizontalAngle = -radToDeg(atan2f(-direction.x, -direction.z));
	normalizeAngles();
}


Vec3 Camera::forward() const
{
	return Vec3(glm::inverse(orientaion()) * Vec4(0, 0, -1, 1));
}


Vec3 Camera::right() const
{
	return Vec3(glm::inverse(orientaion()) * Vec4(1, 0, 0, 1));
}


Vec3 Camera::up() const
{
	return Vec3(glm::inverse(orientaion()) * Vec4(0, 1, 0, 1));
}


///// PRIVATE

Mat4 Camera::totalMatrix() const
{
	return perspectiveMatrix() * viewMatrix();
}


Mat4 Camera::perspectiveMatrix() const
{
	return glm::perspective(mFieldOfView, mAspectRatio, mNearPlane, mFarPlane);
}


Mat4 Camera::viewMatrix() const
{
	return orientaion() * glm::translate(Mat4(), -mPosition);
}


void Camera::normalizeAngles()
{
	mHorizontalAngle = fmodf(mHorizontalAngle, 360.0f);
	if (mHorizontalAngle < 0.0f)
		mHorizontalAngle += 360.0f;

	if (mVerticalAngle > mMaxVerticalAngle)
		mVerticalAngle = mMaxVerticalAngle;
	else if (mVerticalAngle < -mMaxVerticalAngle)
		mVerticalAngle = -mMaxVerticalAngle;
}