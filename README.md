# Smart-Medibox

## About
The **Smart Medibox** is an IoT device designed using ESP32 to help manage medication schedules. It displays the time according to the current timezone, allows setting and canceling alarms for medication times, and includes an automated shading mechanism that adjusts based on temperature and humidity to protect medicines.

<table>
  <tr>
    <td><img src="https://your_image_link_1" alt="Medibox Front View" width="500"></td>
    <td><img src="https://your_image_link_2" alt="Medibox Side View" width="500"></td>
  </tr>
</table>

## Key Features
- **Time Display**: Displays the current time in the correct timezone.
- **Alarm System**: Allows users to set and cancel alarms for taking medications.
- **Smart Shade Adjustment**: Automatically adjusts the shade to protect the medication based on temperature and humidity.
- **Manual Control**: The shade can also be manually controlled for user convenience.

## Technologies Used
- **ESP32**: For microcontroller functionality and Wi-Fi connectivity.
- **RTC Module**: To track time and manage alarms.
- **DHT11 Sensor**: For temperature and humidity readings.
- **OLED Display**: To show the time and alarm information.
- **Servo Motors**: For controlling the shading mechanism.

## Getting Started
1. **Hardware Requirements**:
    - ESP32
    - RTC module (DS3231)
    - DHT11 sensor
    - OLED Display (128x64)
    - Servo motors for shade adjustment

2. **Software Requirements**:
    - Arduino IDE
    - Libraries:
        - Wire.h
        - Adafruit_SSD1306.h
        - RTClib.h
        - DHT.h

3. **Instructions**:
    - Clone this repository: `git clone https://github.com/yourusername/Smart-Medibox.git`
    - Upload the code to your ESP32 using Arduino IDE.
    - Set up the hardware as per the provided schematics.

## License
This project is licensed under the MIT License - see the LICENSE file for details.
