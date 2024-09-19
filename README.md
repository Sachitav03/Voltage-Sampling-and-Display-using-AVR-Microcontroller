# Voltage Sampling and Display using AVR Microcontroller

## Overview
This project involves an embedded system using an AVR microcontroller to sample analog voltage values and display the results on an LCD screen. The system reads input from a 4x4 keypad, allowing the user to start and reset the sampling process. It uses the AVR’s ADC (Analog to Digital Converter) to measure instantaneous voltage values and calculates the minimum, maximum, and average voltages during the sampling period.

## Features
- **Voltage Sampling**: Continuously samples voltage using the ADC and displays the results on the LCD.
- **Keypad Control**: User can control the system via a 4x4 keypad:
  - **'1'**: Start voltage sampling.
  - **'2'**: Stop voltage sampling.
  - **'3'**: Reset all values (min, max, avg).
  - **'4'**: Display the current instantaneous voltage value.
- **Real-time Display**: The LCD shows the following values:
  - Maximum voltage
  - Minimum voltage
  - Instantaneous voltage
  - Average voltage

## Components Used

- **AVR Microcontroller (e.g., ATmega328P)**
- **4x4 Keypad**
- **16x2 LCD Display**
- **External Voltage Source (for ADC input)**

## Key Functions

1. **Keypad Input Handling**:
   - `get_key()`: Reads input from the keypad.
   - `is_pressed()`: Checks if a specific key is pressed.

2. **LCD Display**:
   - `lcd_init()`: Initializes the LCD screen.
   - `print_first()`: Displays a welcome message when the system starts.
   - `print_display()`: Shows the min, max, instant, and average voltages.

3. **ADC Voltage Sampling**:
   - `get_sample()`: Samples the voltage using the ADC.
   - The values are updated in real-time on the LCD screen during the sampling process.

## Flow of the Program

1. **Startup**:
   - The system initializes and displays "Project 4" and "AD Volt" on the LCD.
   
2. **Keypad Commands**:
   - Pressing '1' starts sampling voltage values.
   - Pressing '2' stops the sampling.
   - Pressing '3' resets all values (min, max, avg).
   - Pressing '4' displays the current instantaneous voltage.

3. **Voltage Display**:
   - Once sampling starts, the LCD updates continuously to show:
     - Maximum voltage observed
     - Minimum voltage observed
     - Instantaneous voltage
     - Average voltage over time

## How to Run the Project

1. **Hardware Setup**:
   - Connect the 4x4 keypad to the AVR microcontroller’s GPIO pins.
   - Connect the LCD display to the appropriate pins (refer to the AVR and LCD documentation for pin connections).
   - Connect an external voltage source to the ADC input (e.g., a potentiometer or a fixed voltage).

2. **Compiling and Uploading**:
   - Use AVR Studio or a similar tool to compile the code and upload it to the microcontroller.
   - Make sure the appropriate libraries for handling the LCD and keypad are included.

3. **Operation**:
   - Once the system is powered on, the LCD will display the welcome message.
   - Use the keypad to control the voltage sampling and display, following the instructions mentioned earlier.

## Code Structure

- **`main.c`**: Contains the main program logic for voltage sampling and interaction with the LCD and keypad.
- **`lcd.c` and `lcd.h`**: Handles LCD screen initialization and data display.
- **`avr.c` and `avr.h`**: Provides helper functions for interfacing with the AVR microcontroller's timers and GPIOs.

## Future Improvements

- Add additional features such as sampling rate adjustments.
- Include more options for user interaction through the keypad.
- Optimize the ADC sampling for higher precision measurements.

## License

This project is developed for educational purposes under the course **Computer Science 145: Embedded Systems**.
