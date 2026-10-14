#ifndef MOTORS_H
#define MOTORS_H

void motorsBegin();
void setMotorSpeed(int speed);
void moveForward();
void moveBackward();
void pivotLeft();
void pivotRight();
void stopMotors();

#endif