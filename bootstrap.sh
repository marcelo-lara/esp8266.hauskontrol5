
# repo
git submodule update --init --recursive

## init folders
Nodes=(
    "1.living"
    "2.office" 
    "3.suite"
    "4.office_ac"
)

# set folders
for node in "${Nodes[@]}"; do
    rm -rf $PWD/$node/src/wemos.setup
    rm -rf $PWD/$node/src/Core

    mkdir -p $node/src

    ln -sf $PWD/libs/wemos.setup $PWD/$node/src/wemos.setup
    ln -sf $PWD/libs/Core $PWD/$node/src/Core
done

echo "setup complete!"