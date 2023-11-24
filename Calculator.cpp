#include "Calculator.h"
#include <Arduino.h>

Calculator::Calculator(float dt)
  : dt{dt}
  , rollAngleKalmanFilter{dt, 6.0f, 3.0f, 0.0f, 4.0f}
  , pitchAngleKalmanFilter{dt, 6.0f, 3.0f, 0.0f, 4.0f}
  , heightAndVelocityKalmanFilter{dt, 10.0f, 30.0f, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {1.0f, dt, 0.0f, 1.0f}, {0.5 * dt * dt, dt}, {1.0f, 0.0f}}
{}

Calculator::AngleData Calculator::calculateAngles(const Gyroscope::Data& gyroscopeData, const Accelerometer::Data& accelerometerData, const Magnetometer::Data& magnetometerData)
{
  const float rollAngleMeasure = calculateRollAngle(accelerometerData);
  const float pitchAngleMeasure = calculatePitchAngle(accelerometerData);

  angles.rollAngle = rollAngleKalmanFilter.update(gyroscopeData.rollRate, rollAngleMeasure);
  angles.pitchAngle = pitchAngleKalmanFilter.update(gyroscopeData.pitchRate, pitchAngleMeasure);
  angles.yawAngle = calculateYawAngle(magnetometerData);
  
  return angles;
}

Calculator::HeightAndVelocityData Calculator::calculateHeightAndVelocity(float altitude, float verticalAccelertion)
{
  float measuredHeight = calculateHeight(altitude);
  BLA::Matrix<2, 1> temp = heightAndVelocityKalmanFilter.update(verticalAccelertion, measuredHeight);
  return {temp(0, 0), temp(0, 1)};
}

float Calculator::calculateHeight(float altitude) const
{
  return altitude - barometerGroundLevelAltitude;
}

float Calculator::calculateVerticalAccelertion(const Accelerometer::Data& accelerometerData) const
{
  float rollAnglleRadians = toRadians(angles.rollAngle);
  float pitchAngleRadians = toRadians(angles.pitchAngle);
  return (-sin(pitchAngleRadians) * accelerometerData.x + cos(pitchAngleRadians) * sin(rollAnglleRadians) * accelerometerData.y + cos(pitchAngleRadians) * cos(rollAnglleRadians) * accelerometerData.z - 1) * 9.81 * 100;
}

void Calculator::setZeroYawAngle(float angle)
{
  zeroYawAngle += angle;
  if (zeroYawAngle >= 360.0f)
  {
    zeroYawAngle -= 360.0f;
  }
}

void Calculator::initBarometerGroundLevelAltitude(BMP388& barometer)
{
  float sum = 0.0f;
  for (int i = 0; i < N_BAROMETER_INITIALIZING_MEASUREMENTS; ++i)
  {
    sum += barometer.readAltitude();
    delay(1);
  }
  barometerGroundLevelAltitude = sum / N_BAROMETER_INITIALIZING_MEASUREMENTS;
}

float Calculator::calculateRollAngle(const Accelerometer::Data& accelerometerData)
{
  return toDegrees(atan(accelerometerData.y / sqrt(accelerometerData.x * accelerometerData.x + accelerometerData.z * accelerometerData.z)));
}

float Calculator::calculatePitchAngle(const Accelerometer::Data& accelerometerData)
{
  return toDegrees(-atan(accelerometerData.x / sqrt(accelerometerData.y * accelerometerData.y + accelerometerData.z * accelerometerData.z)));
}

float Calculator::calculateYawAngle(const Magnetometer::Data& magnetometerData) const
{
  float rollAnglleRadians = toRadians(angles.rollAngle);
  float pitchAngleRadians = toRadians(angles.pitchAngle);
  float xm = magnetometerData.x * cos(pitchAngleRadians) + magnetometerData.y * sin(rollAnglleRadians) * sin(pitchAngleRadians) + magnetometerData.z * cos(rollAnglleRadians) * sin(pitchAngleRadians);
  float ym = magnetometerData.y * cos(rollAnglleRadians) - magnetometerData.z * sin(rollAnglleRadians);
  float yawAngle = toDegrees(-atan2(ym, xm));
  yawAngle = (yawAngle >= 0 ? yawAngle : yawAngle + 360.0f) - zeroYawAngle;
  return yawAngle >= 0 ? yawAngle : yawAngle + 360.0f;
}

bool Calculator::isOnTheGround(float horizontalVelocity)
{
  groundPointsSum -= gourndTestPoints[groundTestInd];
  groundPointsSum += horizontalVelocity;
  gourndTestPoints[groundTestInd] = horizontalVelocity;
  ++groundTestInd;
  if (groundTestInd == N_GROUND_TEST_POINTS)
  {
    groundTestInd = 0;
    groundTestReady = true;
  }
  if (groundTestReady)
  {
    float average = groundPointsSum / N_GROUND_TEST_POINTS;
    if (average < GROUND_TEST_THRESHOLD && average > -GROUND_TEST_THRESHOLD)
    {
      ++groundTestScore;
    }
    else
    {
      groundTestScore = 0;
    }
    if (groundTestScore >= N_GROUND_TEST_POINTS)
    {
      return true;
    }
  }
  return false;
}

void Calculator::resetOnTheGroundTest()
{
  for (int i = 0; i < N_GROUND_TEST_POINTS; ++i)
  {
    gourndTestPoints[i] = 0.0f;
  }
  groundTestInd = 0;
  groundPointsSum = 0.0f;
  groundTestReady = false;
  groundTestScore = 0;
}

const Calculator::AngleData& Calculator::getAngles() const
{
  return angles;
}

float Calculator::getAngleDifference(float angle1, float angle2)
{
  float difference = angle1 - angle2;
  if (difference < -180.0f)
  {
    return difference + 360.0f;
  }
  else if (difference > 180.0)
  {
    return difference - 360.0f;
  }
  return difference;
}

float Calculator::toRadians(float angle)
{
  return angle * 3.14159265359f / 180.0f;
}

float Calculator::toDegrees(float angle)
{
  return angle * 180.0 / 3.14159265359f;
}