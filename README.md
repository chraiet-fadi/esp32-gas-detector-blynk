# ESP32 Gas Detector with MQ5, MQ7 and Blynk

An IoT gas detection project based on an ESP32, MQ5 gas sensor, MQ7 carbon monoxide sensor, and the Blynk IoT platform. The system reads both sensors, calculates a clean-air baseline, classifies gas levels as `NORMAL`, `WARNING`, or `DANGER`, and sends live values to a Blynk mobile or web dashboard.

## Features

- ESP32 WiFi connectivity
- MQ5 gas sensor monitoring
- MQ7 carbon monoxide sensor monitoring
- Automatic clean-air baseline calibration on startup
- Raw sensor readings and percentage levels
- Overall system status calculation
- Blynk virtual pin integration
- Serial Monitor diagnostics

## Hardware Requirements

- ESP32 development board
- MQ5 gas sensor module
- MQ7 gas sensor module
- Jumper wires
- Breadboard
- Stable 5V/3.3V power supply, depending on the sensor module requirements

## Wiring

| Component | ESP32 Pin |
| --- | --- |
| MQ5 Analog Output | GPIO 34 |
| MQ7 Analog Output | GPIO 35 |
| VCC | Sensor module required voltage |
| GND | GND |

> Important: ESP32 ADC pins are not 5V tolerant. If your MQ sensor module outputs 5V on the analog pin, use a voltage divider or level shifting before connecting it to the ESP32.

## Blynk Virtual Pins

| Virtual Pin | Data |
| --- | --- |
| V0 | MQ5 raw value |
| V1 | MQ7 raw value |
| V2 | MQ5 gas level percentage |
| V3 | MQ7 gas level percentage |
| V4 | Overall system status |
| V5 | MQ5 status |
| V6 | MQ7 status |

## Setup

1. Install the Arduino IDE.
2. Install ESP32 board support in Arduino IDE.
3. Install the Blynk library.
4. Create a Blynk template for the project.
5. Open `ESP32_Gas_Detector_Blynk.ino`.
6. Replace these placeholders with your own values:

```cpp
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";
```

7. Select your ESP32 board and port.
8. Upload the sketch.
9. Keep the sensors in clean air during startup while the baseline is calculated.

## How It Works

On startup, the ESP32 reads each sensor several times in clean air to calculate baseline values. During operation, it reads MQ5 and MQ7 every two seconds, compares the current readings with the baselines, converts the difference to a percentage, and sends the values to Blynk.

The status logic uses threshold differences:

| Sensor | Warning Difference | Danger Difference |
| --- | ---: | ---: |
| MQ5 | 150 | 350 |
| MQ7 | 120 | 300 |

You may need to tune these values depending on your sensors, wiring, power supply, environment, and required sensitivity.

## Safety Notice

This project is intended for learning and prototyping. MQ sensors require warm-up, calibration, and careful electrical design for reliable measurements. Do not use this project as the only safety device in environments where gas leaks or carbon monoxide exposure could create danger.

## Project Structure

```text
.
├── ESP32_Gas_Detector_Blynk.ino
├── README.md
└── .gitignore
```

## License

This project is provided for educational use. Add a license file before using it in production or distributing modified versions.
