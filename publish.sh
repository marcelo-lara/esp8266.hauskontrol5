#/bin/bash

SKETCH=6.smartplug
PORT=/dev/ttyUSB0


arduino-cli compile --fqbn esp8266:esp8266:d1_mini -p $PORT -u --verify  ./$SKETCH/$SKETCH.ino