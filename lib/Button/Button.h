#ifndef BUTTON_H
#define BUTTON_H

class Button
{
    private:

        uint8_t status;

        uint8_t buttonPin;

    public:

        Button(uint8_t buttonPin);

        void updateButtonFromPin();

        uint8_t getStatus();
};

#endif