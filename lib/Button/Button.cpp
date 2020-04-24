#include <Arduino.h>
#include "Button.h"

Button::Button(uint8_t buttonPin)
{
    pinMode(buttonPin, INPUT);

    this->buttonPin = buttonPin;
}

void Button::updateButtonFromPin()
{
    this->status = digitalRead(this->buttonPin);
}

uint8_t Button::getStatus()
{
    return this->status;
}