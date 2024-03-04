
enum class PowerState : unsigned char
{
  off = 0,
  on = 1,
  none = 2
};

enum class PIDtype : unsigned char
{
  pitch,
  roll,
  yaw,
  speed
};

struct PilotMessage
{
  PowerState powerState;
  float pitchAngle;
  float rollAngle;
  float yawRate;
  float verticalSpeed;
};

struct PidPilotMessage
{
  PowerState powerState;
  float pitchAngle;
  float rollAngle;
  float yawRate;
  float verticalSpeed;
  PIDtype pid;
  float P;
  float I;
  float D;
  float compensation;
};

struct DroneMessage
{
  PowerState powerState;
};