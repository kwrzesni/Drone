#ifndef CALCULATOR
#define CALCULATOR
#include "Gyroscope.h"
#include "Accelerometer.h"
#include "Magnetometer.h"
#include "BMP388.h"
#include "KalmanFilter.h"

class Calculator
{
public:
  struct AngleData
  {
    float rollAngle;
    float pitchAngle;
    float yawAngle;
  };
  struct HeightAndVelocityData
  {
    float height;
    float velocity;
  };
  struct Vector
  {
    float x;
    float y;
    float z;
  };
public:
  Calculator(float dt);
  AngleData calculateAngles(const Gyroscope::Data& gyroscopeData, const Accelerometer::Data& accelerometerData, const Magnetometer::Data& magnetometerData);
  const AngleData& getAngles() const;
  void initBarometerGroundLevelAltitude(BMP388& barometer);
  HeightAndVelocityData calculateHeightAndVelocity(float altitude, float verticalAccelertion);
  float calculateHeight(float altitude) const;
  float calculateVerticalAccelertion(const Accelerometer::Data& accelerometerData) const;
  void setZeroYawAngle(float angle);
  bool isOnTheGround(float horizontalVelocity);
  void resetOnTheGroundTest();
  static float getAngleDifference(float angle1, float angle2);
  static float toRadians(float angle);
  static float toDegrees(float angle);
private:
  static float calculateRollAngle(const Accelerometer::Data& accelerometerData);
  static float calculatePitchAngle(const Accelerometer::Data& accelerometerData);
  float calculateYawAngle(const Magnetometer::Data& magnetometerData) const;
private:
  float dt;
  AngleData angles = {0.0f, 0.0f, 0.0f};
  KalmanFilter<1> rollAngleKalmanFilter;
  KalmanFilter<1> pitchAngleKalmanFilter;
  KalmanFilter<2> heightAndVelocityKalmanFilter;
  float barometerGroundLevelAltitude = 0.0f;
  float zeroYawAngle = 0.0f;
  static constexpr int N_BAROMETER_INITIALIZING_MEASUREMENTS = 2000;
  static constexpr int N_GROUND_TEST_POINTS = 10;
  static constexpr float GROUND_TEST_THRESHOLD = 5.0f;
  float gourndTestPoints[N_GROUND_TEST_POINTS] = {};
  int groundTestInd = 0;
  float groundPointsSum = 0.0f;
  bool groundTestReady = false;
  int groundTestScore = 0;
};

#endif
