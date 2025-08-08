void initializeLimitSwitches() {
    pinMode(X_LIMIT_PIN, INPUT_PULLUP);
    pinMode(Y_LIMIT_PIN, INPUT_PULLUP);
}

boolean getXLimit() {
    return !digitalRead(X_LIMIT_PIN);
}

boolean getYLimit() {
    return !digitalRead(Y_LIMIT_PIN);
}