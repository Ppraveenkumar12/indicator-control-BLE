#define LEFT_LED_PIN 9     // PWM-capable pin
#define RIGHT_LED_PIN 10   // PWM-capable pin

unsigned long lastBlinkTime = 0;
unsigned long lastButtonPressTimeLeft = 0;
unsigned long lastButtonPressTimeRight = 0;
unsigned long lastHazardTime = 0;

bool leftIndicatorOn = false;
bool rightIndicatorOn = false;
bool hazardLightsOn = false;

bool leftLedState = LOW;
bool rightLedState = LOW;

char serialCmd;
unsigned long taskSchedulerTime = 0;

void setup() {
  pinMode(LEFT_LED_PIN, OUTPUT);
  pinMode(RIGHT_LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("System Initialized");
  Serial.println("Press keys:");
  Serial.println("L - Left Indicator");
  Serial.println("R - Right Indicator");
  Serial.println("H - Hazard Lights ON");
  Serial.println("X - Hazard Lights OFF");
}

void loop() {
  unsigned long currentMillis = millis();

  // 100ms Task Scheduler
  if (currentMillis - taskSchedulerTime >= 100) {
    taskSchedulerTime = currentMillis;
    handleBlinking(currentMillis);
  }

  // Handle UART commands
  if (Serial.available()) {
    serialCmd = Serial.read();

    if (serialCmd == 'L') {
      if (currentMillis - lastButtonPressTimeLeft >= 1000) {
        lastButtonPressTimeLeft = currentMillis;

        if (!leftIndicatorOn) {
          leftIndicatorOn = true;
          rightIndicatorOn = false;
          hazardLightsOn = false;
          Serial.println("Left Indicator ON");
          Serial.println("Right Indicator OFF");
          Serial.println("Hazard Lights OFF");
        } else {
          leftIndicatorOn = false;
          Serial.println("Left Indicator OFF");
        }

        publishLEDStatus();
        publishIndicatorStatus();
      }
    }

    else if (serialCmd == 'R') {
      if (currentMillis - lastButtonPressTimeRight >= 1000) {
        lastButtonPressTimeRight = currentMillis;

        if (!rightIndicatorOn) {
          rightIndicatorOn = true;
          leftIndicatorOn = false;
          hazardLightsOn = false;
          Serial.println("Right Indicator ON");
          Serial.println("Left Indicator OFF");
          Serial.println("Hazard Lights OFF");
        } else {
          rightIndicatorOn = false;
          Serial.println("Right Indicator OFF");
        }

        publishLEDStatus();
        publishIndicatorStatus();
      }
    }

    else if (serialCmd == 'H') {
      hazardLightsOn = true;
      leftIndicatorOn = false;
      rightIndicatorOn = false;
      Serial.println("Hazard Lights Activated");
      publishLEDStatus();
      publishIndicatorStatus();
    }

    else if (serialCmd == 'X') {
      if (hazardLightsOn) {
        hazardLightsOn = false;
        analogWrite(LEFT_LED_PIN, 0);
        analogWrite(RIGHT_LED_PIN, 0);
        Serial.println("Hazard Lights Deactivated");
        publishLEDStatus();
        publishIndicatorStatus();
      }
    }
  }
}

// Handles blinking for hazard and indicators
void handleBlinking(unsigned long currentMillis) {
  if (hazardLightsOn) {
    if (currentMillis - lastHazardTime >= 300) {
      lastHazardTime = currentMillis;
      leftLedState = !leftLedState;
      rightLedState = leftLedState;

      analogWrite(LEFT_LED_PIN, leftLedState ? 255 : 0);
      analogWrite(RIGHT_LED_PIN, rightLedState ? 255 : 0);
    }
  } else {
    // Left Indicator
    if (leftIndicatorOn) {
      if (currentMillis - lastBlinkTime >= 300) {
        lastBlinkTime = currentMillis;
        leftLedState = !leftLedState;
        analogWrite(LEFT_LED_PIN, leftLedState ? 255 : 0);
      }
    } else {
      analogWrite(LEFT_LED_PIN, 0);
      leftLedState = LOW;
    }

    // Right Indicator
    if (rightIndicatorOn) {
      if (currentMillis - lastBlinkTime >= 300) {
        lastBlinkTime = currentMillis;
        rightLedState = !rightLedState;
        analogWrite(RIGHT_LED_PIN, rightLedState ? 255 : 0);
      }
    } else {
      analogWrite(RIGHT_LED_PIN, 0);
      rightLedState = LOW;
    }
  }
}

// Publish LED ON/OFF status
void publishLEDStatus() {
  Serial.print("Left LED Status: ");
  Serial.println(leftIndicatorOn || hazardLightsOn ? "ON" : "OFF");

  Serial.print("Right LED Status: ");
  Serial.println(rightIndicatorOn || hazardLightsOn ? "ON" : "OFF");
}

// Publish Indicator State
void publishIndicatorStatus() {
  Serial.print("Left Indicator: ");
  Serial.println(leftIndicatorOn ? "ENGAGED" : "DISENGAGED");

  Serial.print("Right Indicator: ");
  Serial.println(rightIndicatorOn ? "ENGAGED" : "DISENGAGED");

  Serial.print("Hazard Light: ");
  Serial.println(hazardLightsOn ? "ENGAGED" : "DISENGAGED");
}
