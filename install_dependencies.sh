#!/bin/sh

apt-get update
apt-get install -y tar zstd
cd deps
mkdir jsoncpp-1.9.4
tar --zstd -xvf jsoncpp-1.9.4.pkg.tar.zst -C jsoncpp-1.9.4
cp -R jsoncpp-1.9.4/usr/include ../include
cp -R jsoncpp-1.9.4/usr/lib ../lib