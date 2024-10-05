# Smart-Medibox

## Introduction
<p align="justify">
The <strong>Smart Medibox</strong> is an innovative IoT-based device developed using the ESP32 microcontroller. It is designed to assist users in managing their medication schedules more effectively, offering a comprehensive solution for both time management and environmental control within the medibox.
</p>
<p align="justify">
This Medibox combines the best of IoT technologies with healthcare needs, offering a reliable, user-friendly solution to help individuals and caregivers manage medication schedules, improve compliance, and ensure that medicines are stored in optimal conditions. With its ability to automate tasks, send reminders, and maintain the ideal environment for medications, the Smart Medibox adds convenience and safety to daily healthcare routines.
</p>
<table>
  <tr>
    <td><img src="https://github.com/dulan-devinda/Smart-Medibox/blob/main/Images/image1.png?raw=true" alt="Medibox Front View" width="500" height="250"></td>
    <td><img src="https://github.com/dulan-devinda/Smart-Medibox/blob/main/Images/image2.png?raw=true" alt="Medibox Side View"  width="500" height="250"></td>
  </tr>
</table>

## Key Features
<p align="justify">
1. <strong>Time Management and Alarm System</strong>: The Smart Medibox incorporates a real-time clock (RTC) module that tracks the current time based on the local timezone. Users can easily set and cancel alarms to receive timely reminders for taking their medications. The alarms are fully customizable, allowing for multiple alerts throughout the day based on the user's medication schedule. This ensures that users never miss a dose, enhancing adherence to prescribed treatments.
</p>

<p align="justify">
2. <strong>Automated Shading Mechanism</strong>: To maintain the optimal storage conditions for medications, the Smart Medibox includes a temperature and humidity sensor (e.g., DHT11). Based on real-time environmental data, the system automatically adjusts the angle of the shading mechanism to protect the medicines from excessive heat or humidity. This feature is crucial for medicines that require specific storage conditions, especially in varying climates, where external factors can impact their efficacy.
</p>

<p align="justify">
3. <strong>Manual Override</strong>: While the shading mechanism operates autonomously, users have the option to manually control the shade via a simple interface, ensuring flexibility and ease of use. This dual control mechanism ensures that the system remains user-friendly while still offering smart automation features.
</p>

<p align="justify">
4. <strong>Display and User Interface</strong>: The device comes equipped with an OLED display, providing users with clear visibility of the current time, alarms, and environmental conditions within the medibox. The intuitive display ensures that users can easily interact with the system, set their preferences, and monitor their medication schedules.
</p>

<p align="justify">
5. <strong>Remote Monitoring and Control (Phase 2)</strong>: In the advanced phase of the project, Node-RED integration has been implemented to allow for remote monitoring and control of the medibox. Users can track the status of their medication schedules, check environmental conditions, and adjust settings via a web-based dashboard. This feature is particularly useful for caregivers who need to monitor the medication routines of elderly or dependent individuals remotely.
</p>

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
    - Clone this repository: `git clone https://github.com/dulan-devinda/Smart-Medibox.git`
    - Upload the code to your ESP32 using Arduino IDE.
    - Set up the hardware as per the provided schematics.

## License
This project is licensed under the MIT License - see the LICENSE file for details.
