
# repo
git submodule update --init --recursive

## arduino libraries
ln -sf $PWD/libs/microgear-esp8266-arduino /home/$USER/Arduino/libraries/microgear-esp8266-arduino
ln -sf $PWD/libs/IRremoteESP8266 /home/$USER/Arduino/libraries/IRremoteESP8266
ln -sf $PWD/libs/BMx280MI-1 /home/$USER/Arduino/libraries/BMx280MI-1

## init folders
Nodes=(
    "1.living"
    "2.office" 
    "3.suite"
    "4.office_ac"
    "5.stage3"
)

# set folders
for node in "${Nodes[@]}"; do
    rm -rf $PWD/$node/src/wemos.setup
    rm -rf $PWD/$node/src/WebApi
    rm -rf $PWD/$node/src/Core

    mkdir -p $node/src

    ln -sf $PWD/libs/wemos.setup $PWD/$node/src/wemos.setup
    ln -sf $PWD/libs/WebApi $PWD/$node/src/WebApi
    ln -sf $PWD/libs/Core $PWD/$node/src/Core
done

echo "setup complete!"
