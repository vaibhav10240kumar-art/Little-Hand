# Little Hand

A tiny Bluetooth mouse built with an **ESP32**, a **joystick module**, and three push buttons. It shows up as a standard BLE mouse, so it works with phones, tablets, and computers without any driver or app.

## Features

- Joystick moves the cursor, and speed scales with how far you push the stick
- Diagonal movement
- Click button that works as press-and-hold, so you can drag and select text
- Two buttons for vertical scrolling (up and down)
- Stick center is calibrated automatically at power-on
- Deadzone to prevent cursor drift
- Onboard LED blinks while waiting for a connection and lights up during activity
- Non-blocking code (no `delay()` in the main loop), so everything stays responsive

## Hardware

- ESP32 development board (original ESP32 or ESP32-S3; the ESP32-S2 has no Bluetooth)
- Analog joystick module (the usual 5-pin type: GND, VCC, VRx, VRy, SW)
- 3 push buttons (click, scroll up, scroll down)
- Jumper wires and a breadboard (or solder it up)
- Power: USB or a battery

## Wiring

| Part | ESP32 pin |

| Joystick VRx | GPIO 27 |
| Joystick VRy | GPIO 35 |
| Joystick VCC | 3.3V |
| Joystick GND | GND |
| Click button | GPIO 26 and GND |
| Scroll up button | GPIO 14 and GND |
| Scroll down button | GPIO 12 and GND |
| Status LED | GPIO 2 (onboard LED on most boards) |

The buttons use the ESP32's internal pull-up resistors, so each button only needs to connect its pin to GND. No external resistors are required.

> **Note:** GPIO 12 is a boot strapping pin. Don't hold the scroll-down button while powering on or resetting the board, or the ESP32 may fail to boot.

## Software setup

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) and add ESP32 board support (Boards Manager, search for "esp32" by Espressif).
2. Install the **ESP32-BLE-Mouse** library by T-vK (Library Manager, or from [github.com/T-vK/ESP32-BLE-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse)).
3. Open `LittleHand.ino`.
4. Select your ESP32 board and COM port, then click **Upload**.

## Usage

1. Power the ESP32. **Keep the joystick untouched for the first second**, because it calibrates its center position at startup.
2. Open Bluetooth settings on your phone, tablet, or computer and pair with **"Little Hand"**. No PIN is needed.
3. Once connected, the LED stops blinking and the joystick controls the cursor.

 Input -> Action 
 Joystick -> Move cursor 
 Click button (hold) -> Left mouse button down, so you can drag 
 Scroll up button -> Scroll up 
 Scroll down button -> Scroll down 



## Tuning

Adjust these constants at the top of the sketch:

| Constant | What it does |
|----------|--------------|
| `DEADZONE` | Size of the "ignore" zone around the stick center. Raise it if the cursor drifts. |
| `MAX_SPEED` | Fastest cursor speed (pixels per update) at full stick deflection. |
| `MOVE_INTERVAL` | Milliseconds between cursor updates. |
| `SCROLL_INTERVAL` | Milliseconds between scroll steps while a button is held. |

If scrolling feels backwards, swap the `1` and `-1` in the two `mouse.move(0, 0, ...)` scroll lines.

## Troubleshooting

- **Doesn't appear in the Bluetooth list:** check that your board supports BLE and that the sketch uploaded without errors.
- **Won't reconnect after re-flashing or renaming:** "forget" the device in your host's Bluetooth settings, then pair again.
- **Cursor drifts when untouched:** increase `DEADZONE`, and make sure the stick is centered when you power on.
- **Board won't boot:** release the scroll-down button (GPIO 12) before powering on or resetting.
