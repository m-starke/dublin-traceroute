# Building from source

```shell
git clone https://github.com/m-starke/dublin-traceroute.git
```

## Dependencies / Requirements

- cmake
- gcc >= 4.9 OR clang >= 3.8
- libtins 3.4+ built from source
- jsoncpp built from source
- setcap (see note at the end)

### Additional dependencies if cmake fails
- pkg-config if cmake fails on detecting libraries
- cmake-data

## Build

```shell
cd dublin-traceroute
mkdir build
cd build
cmake ..
make
make install
```

Install as root.

## Options for building
```shell
CXX=g++ make
CXX=clang++ make
```

### Note

Note that if setcap is found when installing, it is used to set the CAP_NET_RAW
capability on the dublin-traceroute binary. This allows any regular user to run
dublin-traceroute without root privileges but to be still able to use raw sockets,
necessary to forge the traceroute packets. If setcap is not found, the set-uid bit
is set. If you don't want any of these, you have to run it as root.
