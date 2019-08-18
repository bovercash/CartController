#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"

enum class MotorActions {
    STOPPED=0, 
    FORWARD, 
    BACKWARD, 
    LEFT_FORWARD,
    LEFT_BACKWARD,
    RIGHT_FORWARD,
    RIGHT_BACKWARD
};

class Motor {
public:
    Motor(uint8_t l1, uint8_t l2, uint8_t r1, uint8_t r2, uint8_t ms, uint8_t speed);
    ~Motor();
    void init();
    void setAction(MotorActions action);
    MotorActions getAction();
    uint8_t getStepCount();
    void step();

private:
    void setMotors(int l1, int l2, int r1, int r2);
    uint8_t pin_l1;
    uint8_t pin_l2;
    uint8_t pin_r1;
    uint8_t pin_r2;
    MotorActions currentAction;
    uint8_t maxStep;
    uint8_t currentStep;
    uint8_t spd;
};

#endif