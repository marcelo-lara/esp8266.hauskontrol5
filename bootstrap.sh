
#git submodule update --init --recursive

Nodes=(
    "1.living"
    "2.office" 
    "3.suite"
)


# set folders
for node in "${Nodes[@]}"; do
    mkdir -p $node/src

    ln -sf $PWD/libs/wemos.setup $PWD/$node/src/wemos.setup
    ln -sf $PWD/libs/Core $PWD/$node/src/Core
done

echo "setup complete!"