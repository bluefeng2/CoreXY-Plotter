void initializeServo() {
    penServo.attach(servoPin);

    setPenPosition(0);
    movePenToPosition();
}

boolean getPenPosition() {
    return penPosition;
}

void setPenPosition(boolean position) {
    penPosition = position;
}

void togglePenPosition() {
    penPosition = !penPosition;
}

void movePenToPosition() {
    if (penPosition) {
        penServo.write(penDownAngle);
    } else {
        penServo.write(penUpAngle);
    }
}