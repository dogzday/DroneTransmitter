#ifndef JOYSTICK_H
#define JOYSTICK_H

// TODO finish
class Joystick
{
    private:

        uint16_t x;
        uint16_t y;
        uint8_t button;

        uint8_t xPin;
        uint8_t yPin;
        uint8_t buttonPin;

        uint16_t xAverage;
        uint16_t yAverage;

    public:

        Joystick(uint8_t xPin, uint8_t yPin, uint8_t buttonPin);

        static const uint8_t CALIBRATION = 10;
        static const uint8_t CALIBRATION_DELAY = 10;

        void updateJoystickFromPin();
        void calibrateJoystick();

        uint16_t getX();
        uint16_t getY();
        uint8_t getButton();

        uint16_t getXAverage();
        uint16_t getYAverage();
};

#endif