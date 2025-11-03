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

## First Steps
Before proceeding, make yourself familiar with [Libtropic SDK documentation](https://tropicsquare.github.io/libtropic/latest/).

### Install Dependencies and Requirements

Make sure to have these dependencies installed:
* CMake
    * Ubuntu/Debian: `sudo apt install cmake`
    * Fedora: `sudo dnf install cmake`
* arm-none-eabi-gcc
    * Ubuntu/Debian: `sudo apt install arm-none-eabi-gcc`
    * Fedora: `sudo dnf install arm-none-eabi-gcc`
* OpenOCD (https://openocd.org/pages/getting-openocd.html)
    * Ubuntu/Debian: `sudo apt install openocd`
    * Fedora: `sudo dnf install openocd`

### Clone

This repository must be cloned recursively because it contains submodules with dependencies (such as Libtropic or STM32 libraries).

```bash
git clone https://github.com/tropicsquare/libtropic-stm32.git
cd libtropic-stm32
git submodule update --init --recursive
cd NUCLEO_F439ZI/
```

### Build Examples

All examples are implemented in [Libtropic](https://github.com/tropicsquare/libtropic) repository. For more information about them, refer to the [Examples](https://tropicsquare.github.io/libtropic/latest/get_started/examples/) section in the [Libtropic SDK documentation](https://tropicsquare.github.io/libtropic/latest/)..

Examples are built using following set of commands:

```bash
mkdir build
cd build
cmake -DLT_BUILD_EXAMPLES=1 -DLT_CAL=trezor_crypto ..
make
```
For each example, an ELF binary will be created in the build directory. Once all examples are built, continue with the following chapter.

### Run First Examples
The Nucleo board provides a virtual serial port over USB. To view the output from the examples, connect to this serial port using a terminal emulator (e.g., `minicom`, `screen`, or PuTTY).

At first, it is recommended to run **lt_ex_show_chip_id_and_fwver** example. This example will print among other information the CHIP ID and TROPIC01's firmware versions. To run this example on the Nucleo board, you can use a provided `flash.sh` script:

```sh
./flash.sh build/lt_ex_show_chip_id_and_fwver.elf
```

**Save output of this example for future reference.**

Next, it is recommended to upgrade TROPIC01's internal firmwares, as the new versions fix bugs and guarantee compatibility with the latest version of the Libtropic SDK.

> [!IMPORTANT]
> Using outdated firmware is not recommended. Outdated firmware may not compatible with the latest version of the Libtropic SDK.


To update both internal firmwares to the latest versions, execute the following example:
```bash
./flash.sh build/lt_ex_fw_update.elf
```

After successful execution, your chip will contain the latest firmware and will be compatible with the current Libtropic API.

## Running Advanced Examples

> [!CAUTION]
> Some examples cause **irreversible changes** to the chip. Proceed only after you read [Examples](https://tropicsquare.github.io/libtropic/latest/get_started/examples/) section in the Libtropic documentation and you understand the consequences. The documentation describes which examples are irreversible and what each example does.

Other examples can be run using the same script as examples above:

```bash
./flash.sh <path_to_ELF>
```

> [!IMPORTANT]
> You may encounter issues with examples that establish a Secure Session - refer to [Establishing Your First Secure Channel Session](https://tropicsquare.github.io/libtropic/latest/get_started/default_pairing_keys/#establishing-your-first-secure-channel-session) section in libtropic documentation for more information.

## Building Functional Tests

> [!CAUTION]
> **DANGER!** Functional tests are for internal use only and we provide them only for reference. Some tests can **destroy** your chip. **Do not run the tests** unless you are absolutely sure what you are doing. If you damage your chip with the tests, we are unable to provide any support.

All functional tests are implemented in the [libtropic](https://github.com/tropicsquare/libtropic) repository. For more information about them, refer to the [Functional Tests](https://tropicsquare.github.io/libtropic/latest/for_contributors/functional_tests/) section in the libtropic documentation.

Build all functional tests in one place with the following commands:
```bash
mkdir build
cd build
cmake -DLT_BUILD_TESTS=1 -DLT_CAL=trezor_crypto ..
make
```

For each test, a binary will be created in the build directory (similarly as when building the examples).

> [!IMPORTANT]
> You may encounter issues with tests that establish a Secure Session - refer to [Establishing Your First Secure Channel Session](https://tropicsquare.github.io/libtropic/latest/get_started/default_pairing_keys/#establishing-your-first-secure-channel-session) section in libtropic documentation for more information.

We use CTest for handling functional tests.

To show available tests:
```bash
ctest -N
```

To launch only reversible tests:
```bash
ctest -R _rev_
```

> [!TIP]
> To see all output, use `--verbose` or `-V`.

### Implementation Details and Troubleshooting
CTest uses the `run_test.sh` script, which flashes a binary containing the test and immediately reads the serial port output to evaluate the test results. This script attempts to automatically detect your NUCLEO's serial port. However, if you are using a non-standard setup (e.g., a different ST-Link, special OS configuration) or have multiple NUCLEOs connected to your computer, you must manually specify the path to the serial port during CTest configuration:

```bash
cmake -DLT_BUILD_TESTS=1 -DLT_CAL=trezor_crypto -DSTLINK_UART=<path> ..
```

# FAQ

If you encounter any issues, please check [here](./../FAQ.md) before filing an issue or reaching out to our [support](https://support.desk.tropicsquare.com/).

> [!NOTE]
> Running tests is not officially supported. We do not provide any support for running tests.