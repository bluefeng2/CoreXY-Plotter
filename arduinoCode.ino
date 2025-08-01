#include <MobaTools.h>

const int stepsPerRev = 200;
MoToStepper motorA(stepsPerRev, STEPDIR);
MoToStepper motorB(stepsPerRev, STEPDIR);

const int motorSpeed = 3000;  // steps per second (adjust as needed)
const int motorAccel = 3; // acceleration

// CoreXY position in steps
long xPos = 0;
long yPos = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter target X and Y positions (e.g. 100 200)");

  motorA.attach(42, 38, 44, 40);
  motorB.attach(34, 30, 36, 32);

  motorA.setSpeed(motorSpeed);
  motorB.setSpeed(motorSpeed);

  //motorA.setRampLen(motorAccel);
  //motorB.setRampLen(motorAccel);
}

void loop() {
  static String input = "";
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (input.length() > 0) {
        handleInput(input);
        input = "";
      }
    } else {
      input += c;
    }
  }
}

void handleInput(String input) {
  input.trim();
  int spaceIndex = input.indexOf(' ');
  if (spaceIndex == -1) {
    Serial.println("Invalid input. Format: X Y (e.g., 100 200)");
    return;
  }

  long targetX = input.substring(0, spaceIndex).toInt();
  long targetY = input.substring(spaceIndex + 1).toInt();

  long dx = targetX - xPos;
  long dy = targetY - yPos;

  xPos = targetX;
  yPos = targetY;

  // CoreXY kinematics:
  // A = X + Y
  // B = X - Y
  long deltaA = dx + dy;
  long deltaB = dx - dy;

  Serial.print(deltaA);
  Serial.print(", ");
  Serial.println(deltaB);

  motorA.move(deltaA);
  motorB.move(deltaB);

  Serial.print("Moving to X: ");
  Serial.print(targetX);
  Serial.print(" Y: ");
  Serial.println(targetY);
}
