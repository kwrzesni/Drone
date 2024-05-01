void setup() 
{
  Serial1.begin(1000000);
  delay(3000);
}

float value = 0.0f;
void loop() 
{
  Serial1.write((const uint8_t*)&value, sizeof(value));
  delay(1);
  value += 3;
}
