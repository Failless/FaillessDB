#!/usr/bin/env bash

# cmake installation
DEPS_DIR="${TRAVIS_BUILD_DIR}/deps"
if [ ! -d "$DEPS_DIR" ]; then
  mkdir "${DEPS_DIR}" && cd "${DEPS_DIR}" || exit
fi
#  cd "$DEPS_DIR" || exit
CMAKE_URL="https://github.com/Kitware/CMake/releases/download/v3.15.6/cmake-3.15.6-Linux-x86_64.tar.gz"
wget ${CMAKE_URL}
tar xf cmake-3.15.6-Linux-x86_64.tar.gz
#cp cmake-3.15.6-Linux-x86_64/bin/cmake .
mv cmake-3.15.6-Linux-x86_64 cmake
export PATH=${DEPS_DIR}/cmake/bin:${PATH}

# install gtest
cd "${TRAVIS_BUILD_DIR}/deps" || exit
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
"${DEPS_DIR}"/cmake/bin/cmake ..
make -j 8
sudo make install

# rocksdb installation
cd "$DEPS_DIR" || exit
sudo apt-get install libgflags-dev
sudo apt-get install libsnappy-dev
sudo apt-get install zlib1g-dev
sudo apt-get install libbz2-dev
sudo apt-get install liblz4-devcmake
sudo apt-get install libzstd-dev
git clone https://github.com/facebook/rocksdb.git
cd rocksdb || exit
cp "${TRAVIS_BUILD_DIR}"/*.patch .
git am *.patch
mkdir build && cd build
"${DEPS_DIR}"/cmake/bin/cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_TESTS=OFF -DWITH_TOOLS=OFF
#make rocksdb -j 8 # (TODO: change it on core num)
sudo make install -j 8

# msgpack installation
cd "${TRAVIS_BUILD_DIR}/deps" || exit
git clone https://github.com/msgpack/msgpack-c.git
cd msgpack-c || exit
cmake -DMSGPACK_CXX17=ON .
sudo make -j 8
sudo make install
cd "${TRAVIS_BUILD_DIR}" || exit
