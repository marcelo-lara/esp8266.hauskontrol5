# esp8266.hauskontrol5

## MQTT topics

| branch        |  description      |
|---------------|-------------------|
| /             |  root             |
| hauskontrol   |  controller root  |
| . office      |  controller name  |
| ..  lights    |  device block     |
| ...   main    |  device           |
| ..  ac        |  device block     |
| ...   temp    |  device property  |
| ...   fan     |  device property  |


## Nodes

### Office

| GPIO          |  description      |
|---------------|-------------------|
| 14 (D5)       |  wall switch      |
| 12 (D6)       |  statusLedPin     |
| 13 (D7)       |  relayOutPin      |
|  0 (D3)       |  irSendPin        |
|  5 (D1)       |  bme280_scl       |
|  4 (D2)       |  bme280_sda       |
|  2 (D4)       |  wifiLedPin       |


### Living

| GPIO          |  description      |
|---------------|-------------------|
|  2 // D4      |  wifiLedPin       |
|  5 // D1      |  wallSwitchPin    |
|  2 // D4      |  statusLedPin     |
|  4 // D2      |  out1             |
| 14 // D5      |  out2             |
| 12 // D6      |  out3             |
| 13 // D7      |  out4             |


### Office FX

| GPIO          |  description      |
|---------------|-------------------|
|  12 // D6     |  out2             |
|  13 // D7     |  out3             |
|  14 // D5     |  out1             |


### Living PIR


| GPIO          |  description      |
|---------------|-------------------|
|  5 // D1      |  pir1             |
|  4 // D2      |  pir2             |


## Arduino-cli version

install
```bash
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/local/bin sh
```

set user permissions to use USB
```bash
sudo usermod -a -G tty $USER
sudo addgroup $USER dialout
```

add arduino-cli to path
```bash
...
```

setup
```bash
arduino-cli config init

arduino-cli core update-index --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json


arduino-cli board listall
```

install libraries
```bash
arduino-cli lib install PubSubClient
arduino-cli lib install IRremoteESP8266
arduino-cli lib install BMx280MI
```


build & publish
```bash
#compile
arduino-cli compile --fqbn esp8266:esp8266:d1_mini ./6.smartplug/6.smartplug.ino

#upload
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:d1_mini ./6.smartplug/6.smartplug.ino

# build+upload
arduino-cli compile --fqbn esp8266:esp8266:d1_mini -p /dev/ttyUSB0 -u --verify  ./6.smartplug/6.smartplug.ino
```

monitor
```bash
arduino-cli monitor -p /dev/ttyUSB0 --config baudrate=250000
```
