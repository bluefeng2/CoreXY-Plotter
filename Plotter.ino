#include <MobaTools.h>
#include <Servo.h>


//Pins
const byte stepper1A = 38;
const byte stepper1B = 42;
const byte stepper1C = 40;
const byte stepper1D = 44;
const byte stepper2A = 30;
const byte stepper2B = 34;
const byte stepper2C = 32;
const byte stepper2D = 36;

const int xLimitPin = 9;
const int yLimitPin = 8;

const int servoPin = 11;


//Variables
const int maxX = 900;
const int maxY = 1050;
int curX = 0;
int curY = 0;

const int maxMotorSpeed = 3000;  // Steps per second 
const int motorAccel = 10; // Motor ramp length

boolean penPosition = false; // False = down, True = up
const int penUpAngle = 85;
const int penDownAngle = 120;


// Objects
Servo penServo;

const int stepsPerRev = 200;
MoToStepper motorA(200);
MoToStepper motorB(200);


// Main code
void setup() {
    initializeSteppers();
    initializeServo();
    initializeLimitSwitches();
    Serial.begin(115200);

    homeSteppers();
}

void loop() {
    readSerial();
    movePenToPosition();
}

