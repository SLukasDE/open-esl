#!/bin/sh

#exit

rm -rf build_src ; mkdir -p build_src/src/main
cp -a common4esl/src/main/*   build_src/src/main/
cp -a curl4esl/src/main/*     build_src/src/main/
cp -a esa/src/main/*          build_src/src/main/
cp -a esl/src/main/*          build_src/src/main/
cp -a logbook/src/main/*      build_src/src/main/
cp -a logbook4esl/src/main/*  build_src/src/main/
cp -a mhd4esl/src/main/*      build_src/src/main/
cp -a open-esl/src/main/*     build_src/src/main/
cp -a open-gtx4esl/src/main/* build_src/src/main/
cp -a rdkafka4esl/src/main/*  build_src/src/main/
cp -a sqlite4esl/src/main/*   build_src/src/main/
cp -a tinyxml2/src/main/*     build_src/src/main/
cp -a unixODBC4esl/src/main/* build_src/src/main/
cp -a zsystem/src/main/*      build_src/src/main/
cp -a zsystem4esl/src/main/*  build_src/src/main/


rm -f build_src/src/main/main.cpp
tbuild2 build-file=build-full.cfg clean install
rm -rf build_src
rm -rf build
