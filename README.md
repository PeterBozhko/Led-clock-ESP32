# _Led clock_

This is my pet-project for studying C programming language and observing ESP32.

I want to create a LED clock controlled by an esp32.

For this project I use (hardware):
* ESP32C3 Mini 
* WS2812B LED-strip 
* 3D-printed body with 4 7-segment displays (design by me)
* 18650 battery
* Charge module for battery that can make stable 5v

Software - Pure C and ESPIDF

## RoadMap
0. [x] Design, print, assemble a clock, then connect it to an esp32 
1. [x] Create a led driver for ws2812b
2. [x] Create a driver/program/module for operating a 7-segment display
3. [x] Combine 4 indicators into a clock
4. [ ] Connect to wi-fi and retrive the accurate time
5. [ ] Implement clock control (color, brightness, etc.) via HTTP (or BT?)
6. [ ] Adapt to external brightness (add a light sensor)
7. [ ] Use the battery and monitor its charge level
