#!/usr/bin/env bash

# rocksdb installation
sudo apt-get install libgflags-dev
sudo apt-get install libsnappy-dev
sudo apt-get install zlib1g-dev
sudo apt-get install libbz2-dev
sudo apt-get install liblz4-dev
sudo apt-get install libzstd-dev
git clone https://github.com/facebook/rocksdb.git
cd rocksdb || exit
mkdir build
cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_TESTS=OFF -DWITH_TOOLS=OFF
make all -j 8 # (TODO: change it on core num)
sudo make install

# msgpack installation
cd "${TRAVIS_BUILD_DIR}/deps" || exit
git clone https://github.com/msgpack/msgpack-c.git
cd msgpack-c || exit
cmake -DMSGPACK_CXX17=ON .
sudo make install -j 8
cd "${TRAVIS_BUILD_DIR}" || exit