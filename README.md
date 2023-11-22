# LED Canvas | A project for CMPT 433 Embedded Systems

## Building

### Requirements
```
git
cmake [version >= 3.12]
```

We only support out of source building because we're not monsters.

### Instructions

```
git clone https://csil-git1.cs.surrey.sfu.ca/vbakhshi/433-project.git
mkdir build
cd build
cmake ../433-project
make
```
The make command will install the binaries at `~/cmpt433/public/myApps`.

## Running
The binaries can be found at `~/cmpt433/public/myApps`.

The following binaries are built:
 1. `SFU` : This will display the SFU logo, most recently seen on [r/place](https://www.reddit.com/r/place), on the LED display.
 1. `Tetris` : This will launch a game of Tetris which is displayed on the LED matrix and controlled with with an Xbox controller plugged into the BBG's usb port.
 1. `Mic`: This will output readings from the A2D mic input and will detect loud sounds.

## Other
The `unused` directory contains code that was not used for the presentation, and so the above build and run instructions do not touch it. Part of the `unused` code is a web browser game that was build as a proof of concept, located in `unused/Dinosaur_game/`.

The remaining files, specifically the `.c/.h` files, were an attempt to send inputs from the control inputs via the Zencape hardware through UDP to the browser game. This was started when it seemed that the LED display would not be working as we intended, but the display code was working this part of the project was abondoned in favour of creating something would demonstrate the LED display's capabilities more easily.
