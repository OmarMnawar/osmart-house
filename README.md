# Arduino Smart House 🏡💡

An interactive, remote-controlled smart home prototype powered by Arduino. 

This repository contains the complete C++ firmware for an Arduino-based smart house system featuring a dynamic LCD menu, RGB lighting controls, a piezo buzzer music player, and automated motion-sensing power management.

---

## 📖 The Story & Purpose

This project started with a real-world challenge.
A close friend of mine was completing his *Abitur* (high school graduation) and took an Arduino engineering class.
While he handled the physical concept, he had limited experience with programming and software architecture.
He reached out to me to take over the technical execution.

**I designed, architected, and wrote 100% of the C++ firmware and logic for this project.**
My goal was to build a comprehensive, visual, and highly interactive system that demonstrates how modern home automation works on a microcontroller level, implementing concepts like state-machine menus and power-saving sensor loops while keeping the codebase clean and modular.

---

## 🚀 Key Features

*   **Dynamic LCD Menu System:** A multi-layered screen interface that allows users to navigate through various smart home settings in real-time.
*   **IR Remote Control:** Wireless manipulation of the house features (lights, music, menus) using a standard infrared remote controller.
*   **Automated Motion Sensing (Power Save):** Features an integrated PIR motion sensor. The LCD display automatically goes into sleep mode to save energy and wakes up with a custom "Welcome Home" message the moment movement is detected.
*   **RGB Ambient Lighting:** Customizable mood lighting control directly via the interface.
*   **Piezo Music Player:** A built-in speaker module capable of playing 8-bit chip-tune melodies selected from the LCD menu.

---

## 🛠️ Hardware Components Used

*   **Microcontroller:** Arduino Uno / Mega (or compatible board)
*   **Display:** 16x2 / 20x4 I2C LCD Display Module
*   **Sensors:** PIR Motion Sensor (HC-SR501) & IR Receiver Module
*   **Actuators:** RGB LED, Piezo Buzzer / Speaker
*   **Input:** IR Remote Control
