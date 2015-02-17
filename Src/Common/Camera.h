#include <Common/OpenGLWindow.h>

#include "Helper.h"

class Camera
{
public:
	Camera();

	void resize(unsigned windowWidth, unsigned windowHeight);
	void setPosition(Vec3 position) { mPosition = position; }
	void incPosition(Vec3 positionAmount) { mPosition += positionAmount; }
	Vec3 position() const { return mPosition; }

	void setFieldOfView(float fieldOfView) { mFieldOfView = fieldOfView; }
	float fieldOfView() const { return mFieldOfView; }
	void setNearPlane(float nearPlane) { mNearPlane = nearPlane; }
	float nearPlane() const { return mNearPlane; }
	void setFarPlane(float farPlane) { mFarPlane = farPlane; }
	float farPlane() const { return mFarPlane; }

	void incOrientation(float rightAngle, float upAngle);
	Mat4 orientaion() const;
	void setAspectRatio(float aspectRatio) { mAspectRatio = aspectRatio; }
	float aspectRatio() { return mAspectRatio; }
	void setLodFactor(float lodFactor);

	void lookAt(Vec3 position);
	Vec3 forward() const;
	Vec3 right() const;
	Vec3 up() const;

	Mat4 totalMatrix() const;
	Mat4 perspectiveMatrix() const;
	Mat4 viewMatrix() const;

private:
	Vec3 mPosition;
	float mHorizontalAngle, mVerticalAngle;
	float mFieldOfView;
	float mNearPlane, mFarPlane;
	float mAspectRatio;
	float mMaxVerticalAngle;

private:
	void normalizeAngles();
};