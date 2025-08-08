#include <MobaTools.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//Pins
const int INA1 = 42;
const int INA2 = 38;
const int INA3 = 44;
const int INA4 = 40;
const int INB1 = 34;
const int INB2 = 30;
const int INB3 = 36;
const int INB4 = 32;

const int X_LIMIT_PIN = 9;
const int Y_LIMIT_PIN = 8;

const int SERVO_PIN = 11;

const int ENCODER_CLK = 18;
const int ENCODER_DT = 19;
const int ENCODER_SW = 12;

//Variables
const double maxX = 900.0;
const double maxY = 1050.0;
double curX = 0.0;
double curY = 0.0;
double targetX;
double targetY;

int motorSpeed = 3000;  // Steps per second 
const int motorAccel = 10; // Motor ramp length

boolean penPosition = false; // False = up, True = down
const int penUpAngle = 85;
const int penDownAngle = 120;

const double stepsPerMMA = 1;
const double stepsPerMMB = 1;

volatile int encoderPosition = 0;
int lastClkState;
int lastDtState;

String currentPage = "Home";

// Objects
Servo penServo;

const int stepsPerRev = 200;
MoToStepper motorA(stepsPerRev);
MoToStepper motorB(stepsPerRev);

Adafruit_SSD1306 display(128, 64, &Wire, -1);


// Main code
void setup() {
    initializeSteppers();
    initializeServo();
    initializeLimitSwitches();
    initializeDisplay();
    Serial.begin(115200);

    homeSteppers();
}

void loop() {
    if (currentPage == "Serial") {
        readSerial();
    } else {
        if (!isMoving()) {
            moveToTarget();
        }
    }
    movePenToPosition();
    
    updateDisplay();
}

