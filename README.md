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
