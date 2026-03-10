# Autonomous Obstacle Avoiding Vehicle 🏎️

An Arduino-based autonomous robot designed to navigate through environments while avoiding obstacles in real time. This project demonstrates the integration of hardware components with embedded C++ logic.

## 📋 Project Overview
The core of this project is a decision-making algorithm that processes data from an ultrasonic sensor to control the vehicle's movement. It bridges the gap between raw sensor data and motors.

## 🛠️ Hardware Components
- **Microcontroller:** Arduino Uno 
- **Distance Sensing:** HC-SR04 Ultrasonic Sensor
- **Actuators:** 4x DC Motors & 1x Servo Motor (for sensor scanning)
- **Motor Driver:** L298N Dual H-Bridge
- **Power:** 9V Battery

## ⚙️ Engineering Logic
The system follows a continuous feedback loop:
1. **Measurement:** Measuring the distance to the nearest object using ultrasonic waves.
2. **Threshold Check:** If an object is closer than 20cm, the vehicle stops immediately.
3. **Environmental Scanning:** The servo motor rotates the sensor 90 degrees to the left and right to evaluate the "clearest" path.
4. **Execution:** The logic compares distances and commands the L298N driver to turn the vehicle toward the path with maximum clearance.

## 💻 Code Structure
- The source code is located in the [`src/`](./src/) directory.

## 📸 Demo
*(Coming soon: A video demonstration of the vehicle in action will be uploaded here)*
