# esp8266.hauskontrol5

## hardlink to core

CMD

```
mklink /j C:\00-Electronics\esp8266.hauskontrol5\1.living\src\Core      C:\00-Electronics\esp8266.hauskontrol5\libs\Core
mklink /j C:\00-Electronics\esp8266.hauskontrol5\2.office\src\Core      C:\00-Electronics\esp8266.hauskontrol5\libs\Core
mklink /j C:\00-Electronics\esp8266.hauskontrol5\3.suite\src\Core       C:\00-Electronics\esp8266.hauskontrol5\libs\Core
mklink /j C:\00-Electronics\esp8266.hauskontrol5\4.office_ac\src\Core   C:\00-Electronics\esp8266.hauskontrol5\libs\Core

mklink /j C:\00-Electronics\esp8266.hauskontrol5\1.living\src\wemos.setup      C:\00-Electronics\esp8266.hauskontrol5\libs\wemos.setup
mklink /j C:\00-Electronics\esp8266.hauskontrol5\2.office\src\wemos.setup      C:\00-Electronics\esp8266.hauskontrol5\libs\wemos.setup
mklink /j C:\00-Electronics\esp8266.hauskontrol5\3.suite\src\wemos.setup       C:\00-Electronics\esp8266.hauskontrol5\libs\wemos.setup
mklink /j C:\00-Electronics\esp8266.hauskontrol5\4.office_ac\src\wemos.setup   C:\00-Electronics\esp8266.hauskontrol5\libs\wemos.setup

```

Unix

```
ln -sf $PWD/libs/Core $PWD/1.living/src/Core
ln -sf $PWD/libs/Core $PWD/2.office/src/Core
ln -sf $PWD/libs/Core $PWD/3.suite/src/Core

ln -sf $PWD/libs/wemos.setup $PWD/1.living/src/wemos.setup
ln -sf $PWD/libs/wemos.setup $PWD/2.office/src/wemos.setup
ln -sf $PWD/libs/wemos.setup $PWD/3.suite/src/wemos.setup

```