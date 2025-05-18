# About the project

## Intro to the project
This project intends to combine the real-time measurement of parameters like ECG, Blood Pressure, Pulse, SpO2 and Temperature and provides a machine learning diagnosis for Arrhythmia (irregular heart beeat).

## Requirements of the project
- 12-lead ECG acquisition 
- SpO2 and Pulse via MAX30102 sensor
- Temperature via DS18B20
- BP via the attached sensor OR a self-made circuit
- Machine learning algorithm to diagnose ECG as either normal or irregular (arrhythmia)
- Display of all the parameters and machine learning diagnosis on a GUI (waves for ECG, numerical data for other parameters and text for diagnosis)

## Method
Since the Raspberry Pi doesn't have analog pins, the ECG module was connected to an Arduino UNO but the main requirement here is the use of the Raspberry Pi 4B which will use a 7" LCD display to display the GUI.
