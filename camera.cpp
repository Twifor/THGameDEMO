#include "camera.h"

Camera::Camera(QObject *parent) : QObject(parent)
{
	cameraPos = QVector3D(0.0f, 0.0f, 6.0f);
	cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
	cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
	matrix.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	pitch = yaw = roll = 0.0f;
}

void Camera::moveLeft(float steps)
{
	cameraPos += steps * getLeft();
	update();
}

void Camera::setPos(QVector3D pos)
{
	cameraPos = pos;
}

void Camera::moveRight(float steps)
{
	cameraPos += steps * getRight();
	update();
}

void Camera::moveFront(float steps)
{
	cameraPos += steps * getFront();
	update();
}

void Camera::moveBack(float steps)
{
	cameraPos += steps * getBack();
	update();
}

void Camera::moveUP(float steps)
{
	cameraPos += steps * getUp();
	update();
}

void Camera::moveDown(float steps)
{
	cameraPos += steps * getDown();
	update();
}

void Camera::setPitch(float angle)
{
	pitch = angle;
	QVector3D vec;
	vec.setX(cos(pitch * M_PI / 180.0) * sin(yaw * M_PI / 180.0));
	vec.setY(sin(pitch * M_PI / 180.0));
	vec.setZ(-cos(pitch * M_PI / 180.0) * cos(yaw * M_PI / 180.0));
	cameraFront = vec;

	update();
}

void Camera::setYaw(float angel)
{
	yaw = angel;
	QVector3D vec;
	vec.setX(cos(pitch * M_PI / 180.0) * sin(yaw * M_PI / 180.0));
	vec.setY(sin(pitch * M_PI / 180.0));
	vec.setZ(-cos(pitch * M_PI / 180.0) * cos(yaw * M_PI / 180.0));
	cameraFront = vec;

	update();
}

void Camera::setRoll(float angel)
{
	roll = angel;
	cameraUp.setX(0.0f);
	cameraUp.setY(1.0f);
	cameraUp.setZ(0.0f);
	QVector4D temp = QVector4D(cameraUp, 1.0f);
	QMatrix4x4 m;
	m.rotate(-roll, cameraFront);
	temp = m * temp;
	cameraUp.setX(temp.x());
	cameraUp.setY(temp.y());
	cameraUp.setZ(temp.z());

	update();
}

float Camera::getPitch()
{
	return pitch;
}

float Camera::getYaw()
{
	return yaw;
}

float Camera::getRoll()
{
	return roll;
}

QMatrix4x4 Camera::getMatrix()
{
	return matrix;
}

QVector3D Camera::getPos()
{
	return cameraPos;
}

QVector3D Camera::getFront()
{
	return cameraFront;
}

QVector3D Camera::getUp()
{
	return cameraUp;
}

QVector3D Camera::getRight()
{
	return QVector3D::crossProduct(cameraFront, cameraUp).normalized();
}

QVector3D Camera::getLeft()
{
	return -QVector3D::crossProduct(cameraFront, cameraUp).normalized();
}

QVector3D Camera::getDown()
{
	return -cameraUp;
}

QVector3D Camera::getBack()
{
	return -cameraFront;
}

void Camera::update()
{
	matrix.setToIdentity();
	matrix.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
