# External nimiq miner for CPU

## Build

Build using cmake:
```sh
cmake .
make
```

## Run

Make sure the nimiq node is running, configured to use your pool of choice (but with mining disabled) and RPC open with default setting.
Then just run
```sh
./nimiq-miner
```

To see available options, run
```sh
./nimiq-miner --help
```
