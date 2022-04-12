# 433-project

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

## Running
The binaries will be installed at `~/cmpt433/public/myApps`.

The following binaries are built:
 1. `SFU` : This will display the SFU logo most recently seen on [r/place](https://www.reddit.com/r/place).
 1. `Tetris` : This will launch a game of Tetris which is displayed on the LED matrix and controlled with with an Xbox controller plugged into the BBG usb port.
 1. `Mic`: This will output readings from the A2D mic input and will detect loud sounds.
