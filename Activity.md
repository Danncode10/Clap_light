# Activity

## Project Title

Clap-Activated Lamp Shade

## Project Description

Build an Arduino-based lamp shade prototype that turns ON and OFF using a clap. An RGB LED module provides the light output, and a sound sensor detects hand claps to toggle the lamp state.

For additional points, use 3 different claps to cycle through 3 different colors before turning OFF on the fourth clap.

## Objectives

- Use a sound sensor module to detect clap input.
- Control an RGB LED (or LED strip) as the lamp light source.
- Implement a state machine that toggles the lamp on/off with each clap.
- Extend the logic to cycle through 3 colors using clap count before turning off.

## Required Components

| Quantity | Component | Notes |
| --- | --- | --- |
| 1 | Arduino Uno / compatible board | Main controller |
| 1 | Sound sensor module | Microphone-based clap detector |
| 1 | RGB LED module | Lamp shade light output |
| 3 | 220 ohm resistors | Current-limiting resistors for RGB channels |
| 1 | Lamp shade housing | Prototype shade structure |
| 1 | USB cable | Power and programming |
| 4+ | Jumper wires | Signal and ground connections |

## Pin Assignment

| Arduino Pin | Connected Component | Purpose |
| --- | --- | --- |
| D2 | Sound sensor digital output | Clap detection input |
| D9 | RGB LED Red channel (via 220 ohm) | Red color output |
| D10 | RGB LED Green channel (via 220 ohm) | Green color output |
| D11 | RGB LED Blue channel (via 220 ohm) | Blue color output |
| 5V | Sound sensor VCC | Sensor power |
| GND | Common ground | Shared ground for all modules |

## Expected Behavior

1. **Basic mode:** Each clap toggles the lamp between ON and OFF. The lamp starts OFF.
2. **Additional points mode:** The lamp cycles through 3 colors with each clap:
   - Clap 1: Lamp turns ON (Color 1 — e.g., warm white/yellow)
   - Clap 2: Color 2 (e.g., red)
   - Clap 3: Color 3 (e.g., blue)
   - Clap 4: Lamp turns OFF
   - Cycle repeats from clap 1.

## Build Checklist

- [ ] Components prepared
- [ ] Sound sensor wired and tested
- [ ] RGB LED wired and tested
- [ ] Lamp shade housing assembled
- [ ] Basic ON/OFF clap logic uploaded
- [ ] Extended 3-color clap logic uploaded (bonus)
- [ ] Project tested
- [ ] Documentation updated

## Observations

Record whether the sound sensor reliably detects claps without false triggers. Adjust the sensor sensitivity potentiometer if needed. Verify that each color displays correctly and the lamp turns off after the fourth clap.

## Submission Notes

Submit the Arduino sketch folder `arduino/ClapActivatedLampShade/` and this activity documentation.
