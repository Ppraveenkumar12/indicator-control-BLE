#define LEFT_LED_PIN 9
#define RIGHT_LED_PIN 10

unsigned long lastBlinkTime = 0;

bool leftIndicatorOn = false;
bool rightIndicatorOn = false;
bool hazardLightsOn = false;

bool leftLedState = LOW;
bool rightLedState = LOW;

void setup() {
  pinMode(LEFT_LED_PIN, OUTPUT);
  pinMode(RIGHT_LED_PIN, OUTPUT);

  Serial.begin(9600);
  delay(500);
  Serial.println("=== Vehicle Indicator Control System ===");
  Serial.println("System Initialized!");
  Serial.println("Use the following keys:");
  Serial.println("L - Left Indicator");
  Serial.println("R - Right Indicator");
  Serial.println("H - Activate Hazard Lights");
  Serial.println("X - Deactivate Hazard Lights");
  Serial.println("========================================");
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle blinking (every 300 ms)
  if (currentMillis - lastBlinkTime >= 300) {
    lastBlinkTime = currentMillis;

    if (hazardLightsOn) {
      leftLedState = !leftLedState;
      rightLedState = leftLedState;
      analogWrite(LEFT_LED_PIN, leftLedState ? 255 : 0);
      analogWrite(RIGHT_LED_PIN, rightLedState ? 255 : 0);
    } else {
      if (leftIndicatorOn) {
        leftLedState = !leftLedState;
        analogWrite(LEFT_LED_PIN, leftLedState ? 255 : 0);
      } else {
        analogWrite(LEFT_LED_PIN, 0);
      }

      if (rightIndicatorOn) {
        rightLedState = !rightLedState;
        analogWrite(RIGHT_LED_PIN, rightLedState ? 255 : 0);
      } else {
        analogWrite(RIGHT_LED_PIN, 0);
      }
    }
  }

  // Serial input handling
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    switch (cmd) {
      case 'L':
      case 'l':
        if (!hazardLightsOn) {
          leftIndicatorOn = !leftIndicatorOn;
          rightIndicatorOn = false;
          Serial.println(leftIndicatorOn ? "Left Indicator ON" : "Left Indicator OFF");
        }
        break;

      case 'R':
      case 'r':
        if (!hazardLightsOn) {
          rightIndicatorOn = !rightIndicatorOn;
          leftIndicatorOn = false;
          Serial.println(rightIndicatorOn ? "Right Indicator ON" : "Right Indicator OFF");
        }
        break;

      case 'H':
      case 'h':
        hazardLightsOn = true;
        leftIndicatorOn = false;
        rightIndicatorOn = false;
        Serial.println("Hazard Lights ON");
        break;

      case 'X':
      case 'x':
        hazardLightsOn = false;
        analogWrite(LEFT_LED_PIN, 0);
        analogWrite(RIGHT_LED_PIN, 0);
        Serial.println("Hazard Lights OFF");
        break;

      default:
        Serial.println("Invalid input. Use L, R, H, or X.");
        break;
    }
  }
}
