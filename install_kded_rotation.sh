#!/bin/bash

DIR=$(pwd)
BUILD_DIR=$DIR/build

function prepare_application {
  rm -rf $BUILD_DIR
  mkdir -p $BUILD_DIR
}

function build_application {
  if cd $BUILD_DIR; then
    cmake ../. \
      -DCMAKE_INSTALL_PREFIX=/usr \
      -DCMAKE_BUILD_TYPE=Release \
      -DLIB_INSTALL_DIR=lib \
      -DLIBEXEC_INSTALL_DIR=lib \
      -DKDE_INSTALL_USE_QT_SYS_PATHS=ON

      make
  fi
}

function install_application {
  if cd $BUILD_DIR; then
    read -p "Run sudo make install? [y/n] " INSTALL_CHOICE
    echo ""
    if [[ $INSTALL_CHOICE =~ ^[Yy]$ ]]; then
      sudo make install
      sudo chmod +x /usr/bin/orientation-helper
    fi
  fi
}

function main {
  prepare_application
  build_application
  install_application
}

main

exit 0
