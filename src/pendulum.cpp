#include "pendulum.hpp"

void Pendulum::setMotor(unsigned char i, bool dir, float power)
{
    if (motorFlipped[dofIndex + i])
        dir = !dir;

    digitalWrite(motorDirAPin[dofIndex + i], dir);
    digitalWrite(motorDirBPin[dofIndex + i], !dir);
    if (motorPwmPin[dofIndex + i] != dummyPin)
    {
        power = min(power, motorPowerLimit[dofIndex + i]);
        if (power < motorPowerLimit[dofIndex + i])
            engagedTime[i] = 0;
        else if (++engagedTime[i] >= 36000)
        {
            disengageMotors();
            while (1)
                ;
        }
        ledcWrite(dofIndex + i, power * PWM_MAX);
    }
};