#include <Arduino.h>
#include "Joystick.h"

// TODO finish everything

// TODO exceptions
Joystick::Joystick(uint8_t xPin, uint8_t yPin, uint8_t buttonPin)
{
    pinMode(buttonPin, INPUT_PULLUP);
    
    this->xPin = xPin;
    this->yPin = yPin;
    this->buttonPin = buttonPin;
}

void Joystick::updateJoystickFromPin()
{
    this->x = analogRead(xPin);
    this->y = analogRead(yPin);
    this->button = digitalRead(buttonPin);
}

void Joystick::calibrateJoystick()
{
    uint16_t xSum = 0;
    uint16_t ySum = 0;

    for (int i = 0; i < CALIBRATION; ++i)
    {
        this->updateJoystickFromPin();

        xSum += this->x;
        ySum += this->y;
    }

    this->xAverage = xSum / CALIBRATION;
    this->yAverage = ySum / CALIBRATION;
}

uint16_t Joystick::getX()
{
    return this->x;
}

uint16_t Joystick::getY()
{
    return this->y;
}

uint8_t Joystick::getButton()
{
    return this->button;
}

uint16_t Joystick::getXAverage()
{
    return this->xAverage;
}

uint16_t Joystick::getYAverage()
{
    return this->yAverage;
}
