# SmartGarage

SmartGarage is a microcontroller‑based garage door controller designed to automate the opening and closing of a garage door safely. The project was originally developed out of pure interest and demonstrates the basics of digital I/O, sensor handling and state machines using the Arduino framework.

## Features
- **Remote control**: Open or close the door with a wireless remote.
- **Guard mode**: Triggers sound and LED alarm when movement is noticed inside of the garage while Guard Mode is active.
- **Safety sensors**: Uses limit switches to detect when the door is fully open or closed to prevent damage.
- **Modular design**: Simple code structure makes it easy to adapt the logic for other actuators or sensors.

## Hardware
- Arduino‑compatible microcontroller (e.g., Arduino Uno or Nano)
- Motor driver or relay module for the garage door motor
- Limit switches or magnetic sensors to detect travel endpoints
- Optional RF receiver module and remote transmitter

## Software / Stack
- **Language**: C++ using the Arduino core
- **Libraries**: Only the standard Arduino core is required.  If additional libraries are needed (e.g., for RF receivers), place their ZIP files inside a `library/` folder in this repository.

## Build & Upload
1. Open the main sketch in the Arduino IDE.
2. Select your board and COM port from the **Tools** menu.
3. Click **Verify** to compile.
4. Click **Upload** to flash the firmware onto your Arduino.

Alternatively, using the command‑line **arduino-cli**:
```bash
arduino-cli compile --fqbn arduino:avr:uno Smart_garaj.ino
arduino-cli upload -p <PORT> --fqbn arduino:avr:uno Smart_garaj.ino
```

## Contributing
Feel free to fork this project and adapt it for your own garage door system.  Pull requests are welcome for bug fixes or enhancements.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.
