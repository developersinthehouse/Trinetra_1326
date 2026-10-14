#include "IndicatorServo.h"

Servo indicatorServo;

void setIndicatorAngle(int angle)
{
  angle = constrain(angle, 0, 180);
  indicatorServo.write(angle);
}