
String startPageValues[3] = {"Serial", "Control", "Settings"};
String controlValues[5] = {"Back", "Move X", "Move Y", "Move Pen", "Home Axes"};
String settingsValues[2] = {"Back", "Speed"};
String editValue = "";

void initializeDisplay() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();

    pinMode(ENCODER_CLK, INPUT_PULLUP);
    pinMode(ENCODER_DT, INPUT_PULLUP);
    pinMode(ENCODER_SW, INPUT_PULLUP);
    lastClkState = digitalRead(ENCODER_CLK);
    lastDtState = digitalRead(ENCODER_DT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), updateEncoder, CHANGE);
}

int cursor = 0;
int cursorOffset = 0;
int startIndex = 0;
int endIndex = 0;
int showIndex = 0;
int encoderStart = 0;
int tVal = 0;
void updateDisplay() {
    if (editValue == "") {
        if (currentPage == "Home") {
            startIndex = 0;
            endIndex = 2;
        } else if (currentPage == "Control") {
            startIndex = 0;
            endIndex = 4;
        } else if (currentPage == "Serial") {
            startIndex = 0;
            endIndex = 2;
        } else if (currentPage == "Settings") {
            startIndex = 0;
            endIndex = 1;
        }
    } else {
        if (editValue == "X: ") {
            targetX = tVal + getEncoderPosition() - encoderStart;
        } else if (editValue == "Y: ") {
            targetY = tVal + getEncoderPosition() - encoderStart;
        } else if (editValue == "Speed: ") {
            motorSpeed = tVal + (getEncoderPosition() - encoderStart) * 10;
            setMotorSpeeds(motorSpeed);
        } else if (editValue == "Servo: ") {
            penPosition = (tVal + getEncoderPosition() - encoderStart) % 2;
        }    
    }

    int newCursor = getEncoderPosition() + cursorOffset;

    newCursor = clamp(newCursor, startIndex, endIndex);
    cursorOffset = newCursor - getEncoderPosition();
    cursor = newCursor;

    if (cursor < showIndex) { 
        showIndex = cursor;
    } else if (cursor > showIndex+2) {
        showIndex = cursor - 2;
    }

    if (getEncoderSwitch()) {
        if (editValue == "") {
            String prevPage = currentPage;
            if (currentPage == "Home") {
                if (cursor == 0) {
                    currentPage = "Serial";
                } else if (cursor == 1) {
                    currentPage = "Control";
                } else if (cursor == 2) {
                    currentPage = "Settings";
                }
            } else if (currentPage == "Serial") {
                currentPage = "Home";
                setPenPosition(1);
            } else if (currentPage == "Control") {
                if (cursor == 0) {
                    currentPage = "Home";
                } else if (cursor == 1) {
                    editValue = "X: ";
                    encoderStart = getEncoderPosition();
                    tVal = targetX;
                } else if (cursor == 2) {
                    editValue = "Y: ";
                    encoderStart = getEncoderPosition();
                    tVal = targetY;
                } else if (cursor == 3) {
                    editValue = "Servo: ";
                    encoderStart = getEncoderPosition();
                    tVal = getPenPosition();
                } else if (cursor == 4) {
                    homeSteppers();
                    while (isMoving());
                }
            } else if (currentPage == "Settings") {
                if (cursor == 0) {
                    currentPage = "Home";
                } else if (cursor == 1) {
                    editValue = "Speed: ";
                    tVal = motorSpeed;
                    encoderStart = getEncoderPosition();
                }
            }
            while (getEncoderSwitch());
            if (currentPage != prevPage) {
                showIndex = 0;
                cursorOffset = -getEncoderPosition();
                display.clearDisplay();
            }
        } else {
            while (getEncoderSwitch());
            editValue = "";
        }
    }

    displayPage();
}

void displayPage() {
    display.clearDisplay();

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    if (editValue == "") {
        if (currentPage == "Home") {
            for (int i=showIndex;i<=min(showIndex+2, endIndex);i++) {
                if (i == cursor) {
                    display.print("|");
                } else {
                    display.print(" ");
                }
                display.println(startPageValues[i]);
            }
        } else if (currentPage == "Control") {
            for (int i=showIndex;i<=min(showIndex+2, endIndex);i++) {
                if (i == cursor) {
                    display.print("|");
                } else {
                    display.print(" ");
                }
                display.println(controlValues[i]);
            }
        } else if (currentPage == "Settings") {
            for (int i=showIndex;i<=min(showIndex+2, endIndex);i++) {
                if (i == cursor) {
                    display.print("|");
                } else {
                    display.print(" ");
                }
                display.println(settingsValues[i]);
            }
        } else if (currentPage == "Serial") {
            display.print("X: ");
            display.println(curX);
            display.print("Y: ");
            display.println(curY);
            display.print("Pen: ");
            if (getPenPosition()) {
                display.println("Down");
            } else {
                display.println("Up");
            }
        }
    } else {
        display.println("Back");
        display.println(editValue);
        if (editValue == "X: ") {
            display.print(curX);
        } else if (editValue == "Y: ") {
            display.print(curY);
        } else if (editValue == "Speed: ") {
            display.print(motorSpeed);
        } else if (editValue == "Servo: ") {
            if (getPenPosition()) {
                display.print("Down");
            } else {
                display.print("Up");
            }
        }
    }
    display.display(); 
}

int getEncoderPosition() {
    return encoderPosition/2;
}

boolean getEncoderSwitch() {
    return !digitalRead(ENCODER_SW);
}

void updateEncoder() {
    int currentClkState = digitalRead(ENCODER_CLK);
    int currentDtState = digitalRead(ENCODER_DT);

    if (currentClkState != lastClkState) {
        if (currentDtState != currentClkState) {
            encoderPosition--;
        } else {
            encoderPosition++;
        }
    }

    lastClkState = currentClkState;
}