void readSerial() {
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

    if (input == "HOMING") {
        homeSteppers();
    } else if (input == "GETPOSITION") {
        Serial.print(curX); Serial.print(" "); Serial.println(curY);
    } else {
        int firstSpace = input.indexOf(' ');
        int secondSpace = input.indexOf(' ', firstSpace + 1);

        int a = input.substring(0, firstSpace).toInt();
        int b = input.substring(firstSpace + 1, secondSpace).toInt();
        int c = input.substring(secondSpace + 1).toInt();

        if (!isMoving()) {
            moveToTarget(a, b);
        }
        
        if (c == 1) {
            setPenPosition(true);
        } else {
            setPenPosition(false);
        }
    }
}
