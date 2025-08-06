void initializeLimitSwitches() {
    pinMode(xLimitPin, INPUT_PULLUP);
    pinMode(yLimitPin, INPUT_PULLUP);
}

boolean getXLimit() {
    return !digitalRead(xLimitPin);
}

boolean getYLimit() {
    return !digitalRead(yLimitPin);
}