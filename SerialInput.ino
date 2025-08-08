#define INPUT_BUFFER_SIZE 128

char inputBuffer[INPUT_BUFFER_SIZE];
int inputPos = 0;
bool gotNewline = false;

void readSerial() {
    while (Serial.available() > 0) {
        char c = Serial.read();

        if (c == '\n' || c == '\r') {
            // Mark that a line ended
            gotNewline = true;
        } else {
            // If gotNewline was true, it means we got a new line before processing previous
            // So reset buffer to keep only latest line
            if (gotNewline) {
                inputPos = 0;
                gotNewline = false;
            }
            if (inputPos < INPUT_BUFFER_SIZE - 1) {
                inputBuffer[inputPos++] = c;
            } else {
                inputPos = 0;  // discard if overflow
            }
        }
    }

    // If a newline was received, process the last line
    if (gotNewline && inputPos > 0) {
        inputBuffer[inputPos] = '\0';
        handleInput(inputBuffer);
        inputPos = 0;
        gotNewline = false;
    }
}

void handleInput(const char* input) {
    int a = 0, b = 0, c = 0;
    int matched = sscanf(input, "%d %d %d", &a, &b, &c);

    if (matched == 3) {
        if (!isMoving()) {
            moveToAndSetTarget(a, b);
        }
        setPenPosition(c == 1);
    } else {
        Serial.println("Invalid input");
    }
}

