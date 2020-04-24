// CORE
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

// DEPENDS
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <RF24.h>
#include <nRF24L01.h>

// USER
#include "Joystick.h"
#include "Button.h"

/**
 * =============================================================================================================================
 * =========================================================== NOTES ===========================================================
 * conditional compilation
 * transition to making this project interrupt-based
 * =============================================================================================================================
 **/

// ============================================================================================================================= 
// =============================================================================================================================
// =============================================================================================================================

// PINS
static const uint8_t LEFT_X = A0;
static const uint8_t LEFT_Y = A1;
static const uint8_t RIGHT_X = A2;
static const uint8_t RIGHT_Y = A3;

static const uint8_t LEFT_BUTTON = 2;
static const uint8_t RIGHT_BUTTON = 3;

static const uint8_t UP_BUTTON = 4;
static const uint8_t DOWN_BUTTON = 5;

static const uint8_t CE = 7;
static const uint8_t CSN = 8;

// OTHERS
static const uint8_t SCREEN_WIDTH = 128;
static const uint8_t SCREEN_HEIGHT = 32;

static const uint32_t INIT_DELAY = 100;

// =============================================================================================================================
// =============================================================================================================================
// =============================================================================================================================

// RF24 8 bytes transmission, possibly subject to change
struct Transmission
{
    uint8_t up; // up button
    uint8_t dn; // down button

    // warning: potentiometer ranges 0 to 1023, consider map(...) to fit uint8 (0-255)
    uint8_t lx; // left analog x offsest
    uint8_t ly; // left analog y offsest
    uint8_t rx; // right analog x offsest
    uint8_t ry; // right analog y offsest

    uint8_t lb; // left analog button
    uint8_t rb; // right analog button

    uint8_t sp; // potentiometer
} transmission;

RF24 radio(CE, CSN);
const uint8_t address[6] = "00001";

// font size 1, characters are 8 high (0, 8, 16, 24) for 4 lines
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // -1 is board reset pin

Joystick leftJoystick(LEFT_X, LEFT_Y, LEFT_BUTTON);
Joystick rightJoystick(RIGHT_X, RIGHT_Y, RIGHT_BUTTON);

Button upButton(UP_BUTTON);
Button downButton(DOWN_BUTTON);

void init_Adafruit_SSD1306();
void init_Joysticks();
void init_Buttons();
void init_RF24();
void init_Misc();

void LOG(String);

// =============================================================================================================================
// =============================================================================================================================
// =============================================================================================================================

void setup()
{
    init_Adafruit_SSD1306();
    init_Joysticks();
    init_Buttons();
    init_RF24();
    init_Misc();
}

void loop()
{
    leftJoystick.updateJoystickFromPin();
    rightJoystick.updateJoystickFromPin();
    upButton.updateButtonFromPin();
    downButton.updateButtonFromPin();

    transmission.lb = leftJoystick.getButton();
    transmission.rb = rightJoystick.getButton();

    transmission.up = upButton.getStatus();
    transmission.dn = downButton.getStatus();

    transmission.sp = map(analogRead(A6), 0, 1023, 0, 255);

    transmission.lx = map(leftJoystick.getX(), 0, 1023, 0, 255);
    transmission.ly = map(leftJoystick.getY(), 0, 1023, 0, 255);

    transmission.rx = map(rightJoystick.getX(), 0, 1023, 0, 255);
    transmission.ry = map(rightJoystick.getY(), 0, 1023, 0, 255);

    display.clearDisplay();

    display.setCursor(0, 0);
    display.print("LX: ");
    display.print(transmission.lx);
    display.setCursor(SCREEN_WIDTH / 2, 0);
    display.print("RX: ");
    display.print(transmission.rx);

    display.setCursor(0, 8);
    display.print("LY: ");
    display.print(transmission.ly);
    display.setCursor(SCREEN_WIDTH / 2, 8);
    display.print("RY: ");
    display.print(transmission.ry);

    display.setCursor(0, 16);
    display.print("SPEED: ");
    display.print(transmission.sp);

    display.display();

    radio.write(&transmission, sizeof(Transmission));
}

// =============================================================================================================================
// =============================================================================================================================
// =============================================================================================================================

void init_Adafruit_SSD1306()
{
    display.begin();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    LOG("Initialized display");
}

void init_Joysticks()
{    
    LOG("Calibrating...");
    
    LOG("Calibrating left...");
    leftJoystick.calibrateJoystick();
    LOG("Calibrated left");
    
    LOG("Calibrating right...");
    rightJoystick.calibrateJoystick();
    LOG("Calibrated right");

    LOG("Done calibrating");
}

void init_Buttons()
{
    LOG("Starting buttons...");

    LOG("Started buttons");
}

void init_RF24()
{
    LOG("Begin RF24 module...");

    radio.begin();
    radio.setAutoAck(false);
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate(RF24_250KBPS);
    radio.setChannel(125);
    radio.stopListening();

    LOG("Started RF24 module");
}

void init_Misc()
{
    LOG("Finalizing...");

    LOG("Done! Fly safe.");
}

// =============================================================================================================================
// =============================================================================================================================
// =============================================================================================================================

void LOG(String message)
{
    static int line = 0;

    line %= 5;

    if (line == 0) display.clearDisplay();

    display.setCursor(0, line * 8);

    display.println(message);
    display.display();
    delay(INIT_DELAY);

    ++line;
}