#ifndef DRONE
#define DRONE

#include <Ebyte32.h>
#include <Gyroscope.h>
#include <Accelerometer.h>
#include <Barometer.h>
#include <PID.h>
#include <KalmanFilter.h>
#include <LowPassFilter.h>
#include <CommonDataTypes.h>

class Drone
{
private:
  enum class State
  {
    off = 0,
    userControlled = 1,
    disconnected = 2,
    longTimeDisconnected = 3
  };
public:
  Drone() = default;
  void begin();
  void step();
private:
  void setMotorTimers();
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
  void setMotorsPWM();
  void waitTillEndOfStep();
  void resetPID();
private:
  static float toRadians(float angle);
  static float toDegrees(float angle);
  static float clamp(float value, float min, float max);
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
  Gyroscope gyroscope;
  Accelerometer accelerometer = {{ 0.024943, 0.008254, 0.009014 }, {1.002459, 0.000639, -0.000928, 0.000639, 0.998113, 0.001403, -0.000928, 0.001403, 0.994740}};
  Barometer barometer;
  Ebyte32 ebyte32 = Ebyte32{&Serial3, PB12};

  // Remote control
  const unsigned long DISCONNECT_THRESHOLD = 1000000; // [μs]
  const unsigned long LONG_DISCONNECT_THRESHOLD = 10000000; // [μs]
  const unsigned short PILOT_ADDRESS = 0x2137;
  const unsigned char CHANNEL = 5;
  long lastPilotMessageTime = -999999;  // [μs]
  unsigned long disconnectTime = 0;  // [μs]
  PilotMessage pilotMessage = {};
  DroneMessage droneMessage = {PowerState::off};
  
  // Control loop
  const long STEP_TIME = 2500; // [μs]
  const float DT = STEP_TIME / 1000000.0f; // [s]
  const float NORMAL_MOTOR_SPEED = 0.4f;
  const float MINIMUM_MOTOR_SPEED_TO_SPIN = 100.0f;
  const float ANGLE_PID_LIMIT = 100;
  const float RATE_PID_LIMIT = 400;
  const float VERTICAL_SPEED_PID_LIMIT = 400;
  unsigned long stepStartTime = 0; // [μs]
  PID rollAnglePID = {0.777278712858697f, 0.0006303464886447175f, 0.3887969430515168f, ANGLE_PID_LIMIT};
  PID rollRatePID = {0.763863517684828f, 0.013051722002813675f, 11.56164086104576f, RATE_PID_LIMIT};
  PID pitchAnglePID = {0.74871949521454f, 0.0005852880735682725f, 14.79489546556232f, ANGLE_PID_LIMIT};
  PID pitchRatePID = {0.751135293981279f, 0.00330216719855525f, 0.376393188790278f, RATE_PID_LIMIT};
  PID yawRatePID = {3.0f, 0.01f, 0.0f, RATE_PID_LIMIT};
  PID verticalAccelertionPID = {0.0f, 0.0f, 0.0f, VERTICAL_SPEED_PID_LIMIT};
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
  const int N_GROUND_LEVEL_CALCULATING_MEASUREMENTS = 250;
  const float NOT_MOVING_THRESHOLD = 3.0f;
  const int MINIMUM_NOT_MOVING_STEPS = 20;
  const float UPSIDE_DOWN_THRESHOLD = -0.75f;
  const float LANDING_SPEED = -10.0f;
  Gyroscope::Data gyroscopeData = {};
  Accelerometer::Data accelerometerData = {};
  float barometerData = 0.0f;
  LowPassFilter rollRateLowPassFilter = {13.706f, 11.706f}; // sampling frequency: 250 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter pitchRateLowPassFilter = {13.706f, 11.706f}; // sampling frequency: 250 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter yawRateLowPassFilter = {13.706f, 11.706f}; // sampling frequency: 250 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter accelerometerXLowPassFilter = {13.706f, 11.706f}; // sampling frequency: 250 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter accelerometerYLowPassFilter = {13.706f, 11.706f}; // sampling frequency: 250 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter accelerometerZLowPassFilter = {13.706f, 11.706f}; // sampling frequency: 250 Hz, 3dB cutoff frequency: 10 Hz
  LowPassFilter barometerLowPassFilter = {13.706f, 11.706f}; // sampling frequency: 250 Hz, 3dB cutoff frequency: 10 Hz
  KalmanFilter<1> rollAngleKalmanFilter = {DT, 6.0f, 3.0f, 0.0f, 4.0f};
  KalmanFilter<1> pitchAngleKalmanFilter = {DT, 6.0f, 3.0f, 0.0f, 4.0f};
  KalmanFilter<2> heightAndVelocityKalmanFilter = {DT, 10.0f, 30.0f, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {1.0f, DT, 0.0f, 1.0f}, {0.5 * DT * DT, DT}, {1.0f, 0.0f}};
  float groundLevelAltitude = 0.0f;
  int nNotMovingSteps = 0;
};

#endif
