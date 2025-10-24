# Nucleo F439ZI + TROPIC01 Arduino shield
This folder contains examples of libtropic's usage on the [Nucleo F439ZI](https://os.mbed.com/platforms/ST-Nucleo-F439ZI/) STMicroelectronics development board.

Contributors, please follow the [guidelines](https://github.com/tropicsquare/libtropic-stm32/blob/master/CONTRIBUTING.md).

We recommend using our [Arduino shield](https://www.github.com/tropicsquare/tropic01-arduino-shield-hw). If you are using MikroE's [SECURE TROPIC CLICK](https://www.mikroe.com/secure-tropic-click) or a custom PCB, the example code expects TROPIC01 to be wired to the following pins:
```
|------------|------------------|
|  TROPIC01  |   NUCLEO F439ZI  |
|------------|------------------|
|  GND       |  GND             |
|  3V3       |  3V3             |
|  MISO      |  GPIOA_6         |
|  MOSI      |  GPIOA_7         |
|  SCK       |  GPIOA_5         |
|  CS        |  GPIOD_14        |
|  GPO       |  GPIOF_15        |
|------------|------------------|
```

## Clone

This repository must be cloned recursively because it contains submodules with the libtropic SDK and STM32 drivers.

```bash
git clone https://github.com/tropicsquare/libtropic-stm32.git
cd libtropic-stm32
git submodule update --init --recursive
cd NUCLEO_F439ZI/
```

## Dependencies and Requirements

You need to have:
* CMake (can be installed with `sudo apt install cmake`)
* arm-none-eabi-gcc (can be installed with `sudo apt install gcc-arm-none-eabi`)
* OpenOCD (https://openocd.org/pages/getting-openocd.html)

# Build All Examples

All examples are implemented in the [libtropic](https://github.com/tropicsquare/libtropic) repository. For more information about them, refer to the [Examples](https://tropicsquare.github.io/libtropic/latest/get_started/examples/) section in the libtropic documentation.

Build all examples in one place with the following commands:

```bash
mkdir build
cd build
cmake -DLT_BUILD_EXAMPLES=1 ..
make
```
For each example, an ELF binary will be created in the build directory. Once all examples are built, continue with the following chapter.

# Recommended First Steps

The Nucleo board provides a virtual serial port over USB. To view the output from the examples, connect to this serial port using a terminal emulator (e.g., `minicom`, `screen`, or PuTTY).

 > [!IMPORTANT]
 > Before you start with various examples, we strongly recommend doing two things first:
 > * Read the CHIP ID and TROPIC01's firmware versions and **save the printed output for future reference**.
 > * Update TROPIC01's internal firmware to the latest version.

 > [!TIP]
 > If you want to do a quick connectivity check, for example with the 'Hello World' example, all ELF binaries are available [here](https://github.com/tropicsquare/libtropic-stm32/actions/workflows/build_and_upload_examples.yml) (click on the latest workflow run, scroll down to `Artifacts`, and download an artifact for your board).

## Display and Save Chip ID and Firmware Versions
To display the current versions of the internal firmware and details from the CHIP ID data field, execute the following example:
```bash
./flash.sh build/lt_ex_show_chip_id_and_fwver.elf
```

We recommend saving the printed output for future reference.

## Update Internal Firmware

To update both internal firmware to the latest versions, execute the following example:
```bash
./flash.sh build/lt_ex_fw_update.elf
```

After successful execution, your chip will contain the latest firmware and will be compatible with the libtropic API.

## Building and Running Other Examples

> [!WARNING]
> Some examples cause irreversible changes to the chip. For more details, refer to the [Examples](https://tropicsquare.github.io/libtropic/latest/get_started/examples/) section in the libtropic documentation.

For each built example, a binary is created in the build directory. For example, to run the `lt_ex_hello_world` example:
```bash
./flash.sh build/lt_ex_hello_world.elf
```
In your serial port, you should see output similar to this:
```bash
INFO    [  21] ======================================
INFO    [  22] ==== TROPIC01 Hello World Example ====
INFO    [  23] ======================================
INFO    [  27] Initializing handle
INFO    [  35] Starting Secure Session with key 0
INFO    [  43] 	-------------------------------------------------
INFO    [  46] Sending Ping command with message:
INFO    [  47] 	"This is Hello World message from TROPIC01!!"
INFO    [  55] 	-------------------------------------------------
INFO    [  57] Message received from TROPIC01:
INFO    [  58] 	"This is Hello World message from TROPIC01!!"
INFO    [  59] 	-------------------------------------------------
INFO    [  61] Aborting Secure Session
INFO    [  69] Deinitializing handle
```

> [!IMPORTANT]
> You may encounter issues with examples that establish a Secure Session - refer to [Establishing Your First Secure Channel Session](https://tropicsquare.github.io/libtropic/latest/get_started/default_pairing_keys/#establishing-your-first-secure-channel-session) section in libtropic documentation for more information.

## Building Functional Tests

All functional tests are implemented in the [libtropic](https://github.com/tropicsquare/libtropic) repository. For more information about them, refer to the [Functional Tests](https://tropicsquare.github.io/libtropic/latest/for_contributors/functional_tests/) section in the libtropic documentation.

> [!WARNING]
> Some tests make irreversible changes to the chip, such as writing pairing keys. Those irreversible
> tests contain `_ire_` in their name. On the other hand, reversible tests are marked `_rev_`
> and are generally safe to run, as they only make temporary changes and always perform cleanup.

Build all functional tests in one place with the following commands:
```bash
mkdir build
cd build
cmake -DLT_BUILD_TESTS=1 ..
make
```

For each test, a binary will be created in the build directory (similarly as when building the examples).

> [!IMPORTANT]
> You may encounter issues with tests that establish a Secure Session - refer to [Establishing Your First Secure Channel Session](https://tropicsquare.github.io/libtropic/latest/get_started/default_pairing_keys/#establishing-your-first-secure-channel-session) section in libtropic documentation for more information.

We recommend using CTest for handling functional tests.

To show available tests:
```bash
ctest -N
```

To launch all tests (some of them are irreversible):
```bash
ctest
```

To launch a selected test:
```bash
ctest -R test_name
```

> [!TIP]
> To see all output, use `--verbose`.

### Implementation Details and Troubleshooting
CTest uses the `run_test.sh` script, which flashes a binary containing the test and immediately reads the serial port output to evaluate the test results. This script attempts to automatically detect your NUCLEO's serial port. However, if you are using a non-standard setup (e.g., a different ST-Link, special OS configuration) or have multiple NUCLEOs connected to your computer, you must manually specify the path to the serial port during CTest configuration:

```bash
cmake -DLT_BUILD_TESTS=1 -DSTLINK_UART=<path> ..
```

# FAQ

If you encounter any issues, please check [here](./../FAQ.md) before filing an issue or reaching out to our [support](https://support.desk.tropicsquare.com/).
