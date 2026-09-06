# Hardware Pinout & Wiring Configuration

This document details the microcontroller pin assignments, component specifications, and wiring layout for the 4-LED Serial Controller.


## Pin Assignment Table

| Arduino Pin | Component | 

| **Pin 4**   | LED 1     | 

| **Pin 5**   | LED 2     | 

| **Pin 6**   | LED 3     | 

| **Pin 7**   | LED 4     | 

|   **GND**   | (GND)     |



##  Bill of Materials (BOM)

* **Microcontroller:** 1x Arduino Uno / Nano / Mega
* **LEDs:** 4x Standard 5mm LEDs (Any color)
* **Current-Limiting Resistors:** 4x 220Ω Resistors (or 330Ω)
* **Prototyping:** 1x Half-size Breadboard & Jumper Wires (min 5)


##  Connection Schema

Each LED anode (longer leg) connects to its respective digital output pin on the Arduino, while the cathode (shorter leg) connects to Ground through a 220Ω current-limiting resistor.

```text
  Arduino Pin 4 ───[ Resistor 220Ω ]─── ( Anode ) [ LED 1 ] ( Cathode ) ─── GND
  Arduino Pin 5 ───[ Resistor 220Ω ]─── ( Anode ) [ LED 2 ] ( Cathode ) ─── GND
  Arduino Pin 6 ───[ Resistor 220Ω ]─── ( Anode ) [ LED 3 ] ( Cathode ) ─── GND
  Arduino Pin 7 ───[ Resistor 220Ω ]─── ( Anode ) [ LED 4 ] ( Cathode ) ─── GND
