
#include "motor.h"


Motor::Motor(uint8_t l1, uint8_t l2, uint8_t r1, uint8_t r2, uint8_t ms, uint8_t speed){
    pin_l1 = l1;
    pin_l2 = l2;
    pin_r1 = r1;
    pin_r2 = r2;
    maxStep = ms;
    spd = speed;
}

Motor::~Motor() {}

void Motor::init() {
    pinMode(pin_l1,   OUTPUT);
    pinMode(pin_l2,   OUTPUT);
    pinMode(pin_r1,   OUTPUT);
    pinMode(pin_r2,   OUTPUT);

    digitalWrite(pin_l1, LOW);
    digitalWrite(pin_l2, LOW);
    digitalWrite(pin_r1, LOW);
    digitalWrite(pin_r2, LOW);
    currentStep = 0;
}

void Motor::setAction(MotorActions action){
    currentAction = action;
    currentStep = 0;
}

MotorActions Motor::getAction(){
    return currentAction;
}

uint8_t Motor::getStepCount() {
    return currentStep;
}

void Motor::step(){
    switch(currentAction){
        case MotorActions::FORWARD: 
            setMotors(spd, 0, spd, 0);
            break;
        case MotorActions::BACKWARD:
            setMotors(0, spd, 0, spd);
            break;
        case MotorActions::STOPPED:
            setMotors(0, 0, 0, 0);
            break;
        case MotorActions::LEFT_FORWARD:
            setMotors(spd, 0, 0, 0);
            break;
        case MotorActions::LEFT_BACKWARD:
            setMotors(0, spd, 0, 0);
            break;
        case MotorActions::RIGHT_FORWARD:
            setMotors(0, 0, spd, 0);
            break;
        case MotorActions::RIGHT_BACKWARD:
            setMotors(0, 0, 0, spd);
            break;
    }

    if(currentStep > maxStep){
        setAction(MotorActions::STOPPED);
    }

    currentStep++;
}

void Motor::setMotors(int l1, int l2, int r1, int r2){
    analogWrite(pin_l1, l1);
    analogWrite(pin_l2, l2);
    analogWrite(pin_r1, r1);
    analogWrite(pin_r2, r2);
}
