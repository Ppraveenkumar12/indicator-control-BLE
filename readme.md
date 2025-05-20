# Vehicle Indicator Control Using BLE

This project simulates vehicle indicator control using the Arduino platform. It involves controlling the left and right indicator lights and activating hazard lights when both indicator buttons are pressed simultaneously for 1 second.

## Features
- Left and right indicator toggles using buttons.
- Hazard light activation when both buttons are pressed simultaneously for 1 second.
- Hazard light deactivation when a single button is pressed for 1 second.

## Setup
1. Upload the code to your Arduino board (Arduino UNO or compatible).
2. Test in Tinkercad or using physical hardware.
3. Use the serial monitor to view the UART logs.

## UART Logs
The system will print messages to the serial monitor, such as:
- "Left Indicator ON"
- "Right Indicator OFF"
- "Hazard Lights ON"

## Requirements
- Arduino UNO
- Two push buttons
- Two LEDs (for left and right indicators)
