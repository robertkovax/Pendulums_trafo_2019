#pragma once

#include <Arduino.h>

struct Pendulum
{
    const int mot_1_D = 22;
    const int mot_1_E = 33;
    const int mot_1_PWM = 21;
    const int mot_2_D = 19;
    const int mot_2_E = 32;
    const int mot_2_PWM = 18;
    const int mot_3_D = 16;
    const int mot_3_E = 26;
    const int mot_3_PWM = 4;
    const int mot_4_D = 5;
    const int mot_4_E = 17;
    const int mot_4_PWM = 27;

    int pull_T[4], pull_F[4], hold_T[4], hold_F[4], rew_T[4], rew_F[4];
    byte start_test[4];

    const int freq = 20000;
    const int mot_1_PWM_channel = 0;
    const int mot_2_PWM_channel = 1;
    const int mot_3_PWM_channel = 2;
    const int mot_4_PWM_channel = 3;
    const int resolution = 8;


    void setMotor(unsigned char i, bool dir, float power);
}