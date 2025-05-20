#define LEFT_LED_PIN 9     // PWM pin
#define RIGHT_LED_PIN 10   // PWM pin

unsigned long lastBlinkTime = 0;
unsigned long lastPressTime = 0;
bool leftIndicator = false;
bool rightIndicator = false;
bool hazardMode = false;
bool ledState = false;

void setup() {
  pinMode(LEFT_LED_PIN, OUTPUT);
  pinMode(RIGHT_LED_PIN, OUTPUT);
  Serial.begin(9600);

  Serial.println("---------------");
  Serial.println("System Initialized");
  Serial.println("Enter:");
  Serial.println("L - Left Indicator");
  Serial.println("R - Right Indicator");
  Serial.println("H - Hazard Mode ON");
  Serial.println("X - Deactivate Hazard");
  Serial.println("---------------");
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle Serial Input
  if (Serial.available()) {
    char input = Serial.read();

    Serial.println("---------------");
    Serial.print("Received command: ");
    Serial.println(input);

    if (input == 'L' || input == 'l') {
      if (hazardMode) {
        Serial.println("Deactivating Hazard Mode via Left Button...");
        hazardMode = false;
      }
      if (!leftIndicator) {
        Serial.println("Left Indicator ON");
        Serial.println("Right Indicator OFF");
      } else {
        Serial.println("Left Indicator OFF");
      }
      leftIndicator = !leftIndicator;
      rightIndicator = false;
    }

    else if (input == 'R' || input == 'r') {
      if (hazardMode) {
        Serial.println("Deactivating Hazard Mode via Right Button...");
        hazardMode = false;
      }
      if (!rightIndicator) {
        Serial.println("Right Indicator ON");
        Serial.println("Left Indicator OFF");
      } else {
        Serial.println("Right Indicator OFF");
      }
      rightIndicator = !rightIndicator;
      leftIndicator = false;
    }

    else if (input == 'H' || input == 'h') {
      Serial.println("Hazard Mode Activated");
      hazardMode = true;
      leftIndicator = false;
      rightIndicator = false;
    }

    else if (input == 'X' || input == 'x') {
      Serial.println("Hazard Mode Deactivated");
      hazardMode = false;
    }

    publishStatus();
    Serial.println("---------------");
  }

  // Blink logic every 300ms
  if (currentMillis - lastBlinkTime >= 300) {
    lastBlinkTime = currentMillis;
    ledState = !ledState;

    if (hazardMode) {
      analogWrite(LEFT_LED_PIN, ledState ? 255 : 0);
      analogWrite(RIGHT_LED_PIN, ledState ? 255 : 0);
    } else {
      analogWrite(LEFT_LED_PIN, (leftIndicator && ledState) ? 255 : 0);
      analogWrite(RIGHT_LED_PIN, (rightIndicator && ledState) ? 255 : 0);
    }
  }
}

void publishStatus() {
  Serial.print("Left Indicator: ");
  Serial.println(leftIndicator ? "ENGAGED" : "DISENGAGED");

  Serial.print("Right Indicator: ");
  Serial.println(rightIndicator ? "ENGAGED" : "DISENGAGED");

  Serial.print("Hazard Mode: ");
  Serial.println(hazardMode ? "ACTIVE" : "INACTIVE");
}
