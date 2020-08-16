# esp8266.hauskontrol5

## hardlink to core
CMD

    mklink /j C:\Users\darkangel\Electronics\esp8266.hauskontrol5\2.office\src\Core C:\Users\darkangel\Electronics\esp8266.hauskontrol5\libs\Core
    mklink /j C:\Users\darkangel\Electronics\esp8266.hauskontrol5\1.living\src\Core C:\Users\darkangel\Electronics\esp8266.hauskontrol5\libs\Core

PS

    New-Item -ItemType Junction -Name "./2.office/src/Core" -Target "./libs/Core"