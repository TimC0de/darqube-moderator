# DarQube Moderator
Test task for DarQube company.
Library for filtering swear words.

Tested on Ubuntu WSL

## Requirements
cmake >= 3.9.2

## Installation
- Clone repository
- Run `sudo sh ./install_dependencies.sh` to install dependencies
- Done

## Testing
- `cmake -S . -B build -DCMAKE_BUILD_TYPE=DEBUG`
- `cmake --build build`
- `cd build`
- `ctest`(Run `ctest -V` for verbose testing)

## Install
- `cmake -S . -B build -DCMAKE_BUILD_TYPE=RELEASE`
- `cmake --build build`
- `cmake --install build --prefix bin`
