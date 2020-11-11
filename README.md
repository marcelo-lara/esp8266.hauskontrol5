# esp8266.hauskontrol5

## hardlink to core

CMD

```
mklink /j C:\Users\darkangel\Electronics\esp8266.hauskontrol5\1.living\src\Core C:\Users\darkangel\Electronics\esp8266.hauskontrol5\libs\Core
mklink /j C:\Users\darkangel\Electronics\esp8266.hauskontrol5\2.office\src\Core C:\Users\darkangel\Electronics\esp8266.hauskontrol5\libs\Core
mklink /j C:\Users\darkangel\Electronics\esp8266.hauskontrol5\3.suite\src\Core C:\Users\darkangel\Electronics\esp8266.hauskontrol5\libs\Core
```

Unix

```
ln -sf $PWD/libs/Core $PWD/1.living/src/Core
ln -sf $PWD/libs/Core $PWD/2.office/src/Core
ln -sf $PWD/libs/Core $PWD/3.suite/src/Core

```