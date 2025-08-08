void initializeSteppers() {
    motorA.attach(INA1, INA2, INA3, INA4);
    motorB.attach(INB1, INB2, INB3, INB4);

    motorA.setSpeed(motorSpeed);
    motorB.setSpeed(motorSpeed);

    motorA.setRampLen(motorAccel);
    motorB.setRampLen(motorAccel);

    motorA.attachEnable(5);
    motorB.attachEnable(5);
}

double clamp(double x, double low, double top) {
    return min(max(x, low), top);
}

double mmToSteps(double dist, double steps) {
    return dist * steps;
}

void moveToTarget() {
    targetX = clamp(targetX, 0, maxX);
    targetY = clamp(targetY, 0, maxY);
    
    double dX = targetX - curX;
    double dY = targetY - curY;

    moveSteppers(dX, dY);
}

void moveToAndSetTarget(double x, double y) {
    targetX = x;
    targetY = y;

    targetX = clamp(targetX, 0, maxX);
    targetY = clamp(targetY, 0, maxY);
    
    double dX = targetX - curX;
    double dY = targetY - curY;

    moveSteppers(dX, dY);
}

void setTarget(double x, double y) {
    targetX = x;
    targetY = y;
}

void moveSteppers(double dX, double dY) {
    curX += dX;
    curY += dY;

    double deltaA = dX + dY;
    double deltaB = dX - dY;

    double deltaASteps = mmToSteps(deltaA, stepsPerMMA);
    double deltaBSteps = mmToSteps(deltaB, stepsPerMMB);

    motorA.move(deltaASteps);
    motorB.move(deltaBSteps);
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
    curX = 0.0;
    
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
    curY = 0.0;
    
    setMotorSpeeds(motorSpeed);
}