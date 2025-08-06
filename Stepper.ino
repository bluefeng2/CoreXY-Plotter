void initializeSteppers() {
    motorA.attach(42, 38, 44, 40);
    motorB.attach(34, 30, 36, 32);

    motorA.setSpeed(maxMotorSpeed);
    motorB.setSpeed(maxMotorSpeed);

    motorA.setRampLen(motorAccel);
    motorB.setRampLen(motorAccel);

    motorA.attachEnable(5);
    motorB.attachEnable(5);
}

void moveToTarget(int targetX, int targetY) {
    targetX = min(targetX, maxX);
    targetX = max(targetX, 0);
    targetY = min(targetY, maxY);
    targetY = max(targetY, 0);
    
    int dX = targetX - curX;
    int dY = targetY - curY;

    int deltaA = dX + dY;
    int deltaB = dX - dY;

    motorA.move(deltaA);
    motorB.move(deltaB);

    curX = targetX;
    curY = targetY;
}

void moveSteppers(int dX, int dY) {
    curX += dX;
    curY += dY;

    int deltaA = dX + dY;
    int deltaB = dX - dY;

    motorA.move(deltaA);
    motorB.move(deltaB);
}

void setMotorSpeeds(int speed) {
    motorA.setSpeed(speed);
    motorB.setSpeed(speed);
}

boolean isMoving() {
    return motorA.moving() >= 1 or motorB.moving() >= 1;
}

int initalHomeSpeed = 3000;
int backoffHomeSpeed = 100;
void homeSteppers() {
    setMotorSpeeds(initalHomeSpeed);
    while (!getXLimit()) {
        moveSteppers(-10, 0);
    }
    setMotorSpeeds(backoffHomeSpeed);
    moveSteppers(25, 0);
    while (isMoving()) {}
    while (!getXLimit()) {
        moveSteppers(-10, 0);
    }
    curX = 0;
    
    setMotorSpeeds(initalHomeSpeed);
    while (!getYLimit()) {
        moveSteppers(0, -10);
    }
    setMotorSpeeds(backoffHomeSpeed);
    moveSteppers(0, 25);
    while (isMoving()) {}
    while (!getYLimit()) {
        moveSteppers(0, -10);
    }
    curY = 0;
    
    setMotorSpeeds(maxMotorSpeed);
}