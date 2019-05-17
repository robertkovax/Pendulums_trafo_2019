#pragma once
#include <Arduino.h>

class Motor
{
private:
  int mot_Dir;
  int mot_EN;
  int mot_PWM_channel;

public:
  Motor(int en, int dir, int channel) : mot_EN(en), mot_Dir(dir), mot_PWM_channel(channel){};

  static void motorSetup(Motor *motors[])
  {
    const int mot_Dir[4] = {22, 19, 16, 5};
    const int mot_EN[4] = {33, 32, 26, 17};
    const int mot_PWM[4] = {21, 18, 4, 27};
    const int mot_PWM_channel[4] = {1, 2, 3, 4};

    for (int i = 0; i < 4; i++)
    {
      pinMode(mot_EN[i], OUTPUT);
      pinMode(mot_Dir[i], OUTPUT);
      pinMode(mot_PWM[i], OUTPUT);
      ledcAttachPin(mot_PWM[i], mot_PWM_channel[i]);
      motors[i] = new Motor(mot_EN[i], mot_Dir[i], mot_PWM_channel[i]);
    }
  };

  void setMotor(int en, int force)
  {
    if (en == 1)
    {
      digitalWrite(mot_EN, HIGH);
    }
    else
    {
      digitalWrite(mot_EN, LOW);
    }

    digitalWrite(mot_Dir, HIGH ? force > 0 : LOW);
    ledcWrite(mot_PWM_channel, abs(force));
  };
};
