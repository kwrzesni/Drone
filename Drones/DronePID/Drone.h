#ifndef DRONE
#define DRONE

#include <Ebyte32.h>
#include <Gyroscope.h>
#include <Accelerometer.h>
#include <Barometer.h>
#include <PID.h>
#include <KalmanFilter.h>
#include <LowPassFilter.h>
#include <Motor.h>
#include <CommonDataTypes.h>

class Drone
{
private:
  enum class State
  {
    off,
    userControlled
  };
public:
  Drone() = default;
  void begin();
  void step();
private:
  void readGyroscopeData();
  void readAccelerometerData();
  void readBarometerData();
  void calculateDroneGroundLevel();
  void handleSensors();
  void calculateRollAndPitchAngles();
  void calculateAltitudeAndVerticalMovment();
  void handleRemoteControl();
  void handleStateChanges();
  bool isOnTheGround();
  void resetOnTheGroundTest();
  bool isUpsideDown();
  void setMotorsSpeed();
  void waitTillEndOfStep();
  void resetPID();
private:
  static float linearizeThrottle(double throttle);
  static float toRadians(float angle);
  static float toDegrees(float angle);
private:
  // Drone state
  State state = State::off;
  bool isConnected = false;
  float rollRate = 0.0f;
  float pitchRate = 0.0f;
  float yawRate = 0.0f;
  float accelerometerX = 0.0f;
  float accelerometerY = 0.0f;
  float accelerometerZ = 0.0f;
  float rollAngle = 0.0f;
  float pitchAngle = 0.0f;
  float barometerAltitude = 0.0f;
  float altitude = 0.0f;
  float verticalSpeed = 0.0f;
  float verticalAccelertion = 0.0f;
  
  // Peripherals
  Gyroscope gyroscope = {-5.45f, 5.23f, 6.45f};
  Accelerometer accelerometer = {{ 0.024943, 0.008254, 0.009014 }, {1.002459, 0.000639, -0.000928, 0.000639, 0.998113, 0.001403, -0.000928, 0.001403, 0.994740}};
  Barometer barometer;
  Ebyte32 ebyte32 = Ebyte32{&Serial3, PB12};
  Motor motor0 = Motor{PA2};
  Motor motor1 = Motor{PA0};
  Motor motor2 = Motor{PA1};
  Motor motor3 = Motor{PA3};

  // Remote control
  const unsigned long DISCONNECT_THRESHOLD = 500000; // [μs]
  const unsigned short PILOT_ADDRESS = 0x2137;
  const unsigned char CHANNEL = 5;
  long lastPilotMessageTime = -999999;  // [μs]
  unsigned long disconnectTime = 0;  // [μs]
  PidPilotMessage pidPilotMessage = {};
  DroneMessage droneMessage = {PowerState::off};
  
  // Control loop
  const long STEP_TIME = 6000; // [μs]
  const float DT = STEP_TIME / 1000000.0; // [s]
  unsigned long stepStartTime = 0; // [μs]
  PID rollRatePID = {DT, 0, 0, 0};
  PID pitchRatePID = {DT, 0, 0, 0};
  PID yawRatePID = {DT, 0, 0, 0};
  PID verticalAccelertionPID = {DT, 0, 0, 0};
  float targetRollRate = 0.0f;
  float targetPitchRate = 0.0f;
  float targetYawRate = 0.0f;
  float targetRollAngle = 0.0f;
  float targetPitchAngle = 0.0f;
  float targetVerticalSpeed = 0.0f;
  float targetVerticalAccelertion = 0.0f;
  float motor0Speed = 0.0f;
  float motor1Speed = 0.0f;
  float motor2Speed = 0.0f;
  float motor3Speed = 0.0f;

  // Data processing
  const int N_GROUND_LEVEL_CALCULATING_MEASUREMENTS = 2000;
  const float NOT_MOVING_THRESHOLD = 5.0f;
  const int MINIMUM_NOT_MOVING_STEPS = 10;
  const float UPSIDE_DOWN_THRESHOLD = -0.75f;
  const float LANDING_SPEED = -10.0f;
  Gyroscope::Data gyroscopeData = {};
  Accelerometer::Data accelerometerData = {};
  float barometerData = 0.0f;
  LowPassFilter rollRateLowPassFilter = {6.221, 4.221}; // sampling frequency: 166 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter pitchRateLowPassFilter = {6.221, 4.221}; // sampling frequency: 166 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter yawRateLowPassFilter = {6.221, 4.221}; // sampling frequency: 166 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter accelerometerXLowPassFilter = {6.221, 4.221}; // sampling frequency: 166 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter accelerometerYLowPassFilter = {6.221, 4.221}; // sampling frequency: 166 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter accelerometerZLowPassFilter = {6.221, 4.221}; // sampling frequency: 166 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter barometerLowPassFilter = {6.221, 4.221}; // sampling frequency: 166 Hz, 3dB cutoff frequency: 10 Hz
  KalmanFilter<1> rollAngleKalmanFilter = {DT, 6.0f, 3.0f, 0.0f, 4.0f};
  KalmanFilter<1> pitchAngleKalmanFilter = {DT, 6.0f, 3.0f, 0.0f, 4.0f};
  KalmanFilter<2> heightAndVelocityKalmanFilter = {DT, 10.0f, 30.0f, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {1.0f, DT, 0.0f, 1.0f}, {0.5 * DT * DT, DT}, {1.0f, 0.0f}};
  float groundLevelAltitude = 0.0f;
  int nNotMovingSteps = 0;
  float rollCompensation = 0.0f;
  float pitchCompensation = 0.0f;
};

#endif
