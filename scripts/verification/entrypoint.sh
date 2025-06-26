#!/bin/bash
set -e

echo "Libtropic testing environment started!"

cd /work
mkdir -p build
cd build

echo "Creating Python virtual environment..."
python3 -m venv .venv
source .venv/bin/activate

echo "Gathering Python dependencies..."
pip3 install -r /work/repo/libtropic/scripts/test_runner/requirements.txt

echo "Launching test..."
cmake -DLT_BUILD_TESTS=1 /work/repo/NUCLEO_F439ZI
make
ctest --verbose 

# TODO
# Add NUCLEO_L432_KC and all new STM32 platforms here.

echo "CTest finished! Leaving Libtropic testing environment."
