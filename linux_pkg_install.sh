#!/usr/bin/env bash

# cmake installation
DEPS_DIR="${TRAVIS_BUILD_DIR}/deps"
if [ ! -d "$DEPS_DIR" ]; then
  mkdir "${DEPS_DIR}" && cd "${DEPS_DIR}" || exit

#  cd "$DEPS_DIR" || exit
  CMAKE_URL="https://github.com/Kitware/CMake/releases/download/v3.15.6/cmake-3.15.6-Linux-x86_64.tar.gz"
  wget ${CMAKE_URL}
  tar xf cmake-3.15.6-Linux-x86_64.tar.gz
  #cp cmake-3.15.6-Linux-x86_64/bin/cmake .
  mv cmake-3.15.6-Linux-x86_64 cmake
  export PATH=${DEPS_DIR}/cmake/bin:${PATH}

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
  mkdir build && cd build
  "${DEPS_DIR}"/cmake/bin/cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_TESTS=OFF -DWITH_TOOLS=OFF
  make all -j 8 # (TODO: change it on core num)
  sudo make install

  # msgpack installation
  cd "${TRAVIS_BUILD_DIR}/deps" || exit
  git clone https://github.com/msgpack/msgpack-c.git
  cd msgpack-c || exit
  cmake -DMSGPACK_CXX17=ON .
  sudo make install -j 8
  cd "${TRAVIS_BUILD_DIR}" || exit
fi

# install gtest
sudo apt-get install libgtest-dev
#sudo apt-get install cmake # install cmake
# shellcheck disable=SC2164
cd /usr/src/gtest
sudo "${DEPS_DIR}"/cmake/bin/cmake CMakeLists.txt
sudo make

#copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder
sudo cp *.a /usr/lib

