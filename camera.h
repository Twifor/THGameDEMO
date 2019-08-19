#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QMatrix4x4>

class Camera : public QObject
{
	Q_OBJECT
public:
	explicit Camera(QObject *parent = nullptr);
	void moveLeft(float steps);
	void moveRight(float steps);
	void moveFront(float steps);
	void moveBack(float steps);
	void moveUP(float steps);
	void moveDown(float steps);
	void setPitch(float angle);
	void setYaw(float angel);
	void setRoll(float angel);

	void setPos(QVector3D pos);

	float getPitch();
	float getYaw();
	float getRoll();

	QMatrix4x4 getMatrix();
	QVector3D getPos();
	QVector3D getFront();
	QVector3D getUp();
	QVector3D getRight();
	QVector3D getLeft();
	QVector3D getDown();
	QVector3D getBack();

protected:
	void update();

signals:

public slots:

private:
	QMatrix4x4 matrix;
	QVector3D cameraPos, cameraFront, cameraUp;
	float pitch, yaw, roll;

};

#endif // CAMERA_H
