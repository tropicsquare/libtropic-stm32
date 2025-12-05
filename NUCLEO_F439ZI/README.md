# Nucleo F439ZI with Arduino Shield Tutorial
This folder contains examples demonstrating how to use Libtropic on the [Nucleo F439ZI](https://os.mbed.com/platforms/ST-Nucleo-F439ZI/) STMicroelectronics development board using our [TROPIC01 Arduino shield](https://github.com/tropicsquare/tropic01-arduino-shield-hw). Follow the link to get more details about this shield, including schematics, design files, and manufacturing data.

**We recommend using the [Arduino shield](https://www.github.com/tropicsquare/tropic01-arduino-shield-hw)**. If you are using MikroE's [SECURE TROPIC CLICK](https://www.mikroe.com/secure-tropic-click) or a custom PCB, the example code expects TROPIC01 to be wired to the following pins:
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

Contents:

- [Nucleo F439ZI with Arduino Shield Tutorial](#nucleo-f439zi-with-arduino-shield-tutorial)
  - [First Steps](#first-steps)
    - [Install Dependencies](#install-dependencies)
    - [System Setup](#system-setup)
      - [GNU/Linux](#gnulinux)
    - [Clone the libtropic-stm32 Repository](#clone-the-libtropic-stm32-repository)
    - [Build Examples](#build-examples)
    - [Run a Basic Example: Read CHIP ID and firmware versions](#run-a-basic-example-read-chip-id-and-firmware-versions)
    - [Update Internal Firmware](#update-internal-firmware)
  - [Running Advanced Examples](#running-advanced-examples)
  - [Building Functional Tests](#building-functional-tests)
    - [Implementation Details and Troubleshooting](#implementation-details-and-troubleshooting)
  - [FAQ](#faq)

## First Steps
> [!IMPORTANT]
> **Do not skip these steps**. You will gather basic information about the chip (which you will need for any eventual support) and update your chip's firmware, which will guarantee compatibility with the latest Libtropic API.

Before proceeding, familiarize yourself with the [Libtropic SDK documentation](https://tropicsquare.github.io/libtropic/latest/).

> [!TIP]
> For this tutorial, we recommend using Linux machine if you have one available.

### Install Dependencies
Make sure to have these dependencies installed:

* CMake
  * Ubuntu/Debian: `sudo apt install cmake`
  * Fedora: `sudo dnf install cmake`
  * Windows: [official web](https://cmake.org/download/)
* arm-none-eabi-gcc
  * Ubuntu/Debian: `sudo apt install arm-none-eabi-gcc`
  * Fedora: `sudo dnf install arm-none-eabi-gcc`
  * Windows: [official web](https://developer.arm.com/downloads/-/gnu-rm)
* OpenOCD (https://openocd.org/pages/getting-openocd.html)
  * Ubuntu/Debian: `sudo apt install openocd`
  * Fedora: `sudo dnf install openocd`
  * Windows: [build from source](https://openocd.org/doc-release/README.Windows) or search for pre-built binaries
* Serial monitor of your choice
  * On GNU/Linux (Ubuntu/Debian/Fedora/...) you can use `minicom`, `screen` or GTKTerm (recommended).
  * Windows: [PuTTY](https://putty.org/index.html) or [RealTerm](https://sourceforge.net/projects/realterm/)

MbedTLS 4.0.0 which we use in this repository requires:

* Recent Python
* The following Python packages:
  * jinja2
  * jsonschema

The recommended method is to use Python virtual environment to install those packages. Instructions for Linux:
```shell
python3 -m venv .venv
source .venv/bin/activate
pip3 install --upgrade pip
pip3 install jinja2 jsonschema
```

### System Setup

#### GNU/Linux
To upload the example programs, you need to have access to USB devices (usually, you must be a member of the `plugdev` group). Also, permissions (udev rules) have to be correctly configured, refer to the OpenOCD documentation.

As the output of examples is printed to the serial port, you need an access to it. Either run the serial port monitor as root, or you have to add yourself to a group with access to serial ports. Usually, you must be a member of the `dialout` group.

```bash
# Check if you are in the plugdev and the dialout groups
# Required groups can be different, check the documentation of your 
# Linux distribution
groups
# Add yourself to each group you are not in
sudo usermod -aG plugdev "$USER"
sudo usermod -aG dialout "$USER"
```

### Clone the libtropic-stm32 Repository

This repository must be cloned recursively because it contains submodules with dependencies (such as Libtropic or STM32 libraries).

```bash
git clone https://github.com/tropicsquare/libtropic-stm32.git
cd libtropic-stm32
git submodule update --init --recursive
cd NUCLEO_F439ZI/
```

### Build Examples

All examples are implemented in the [Libtropic](https://github.com/tropicsquare/libtropic) repository. For more information about them, refer to the [Examples](https://tropicsquare.github.io/libtropic/latest/get_started/examples/) section in the Libtropic SDK documentation.

Examples are built using the following commands:

```bash
mkdir build
cd build
cmake -DLT_BUILD_EXAMPLES=1 -DLT_CAL=mbedtls_v4 ..
make
```

> [!NOTE]
> `-DLT_CAL` is used to select Cryptography Function Provider. We provide multiple — refer to the [Supported Cryptographic Functionality Providers](https://tropicsquare.github.io/libtropic/latest/other/supported_cfps/) section in the Libtropic SDK documentation.  

For each example, an ELF binary will be created in the build directory. Once all examples are built, **continue with the following section**.

### Run a Basic Example: Read CHIP ID and firmware versions
The Nucleo board provides a virtual serial port over USB. To view the output from the examples, connect to this serial port using your preferred serial monitor you have prepared in [Install Dependencies](#install-dependencies).

First, it is recommended to run the **lt_ex_show_chip_id_and_fwver** example. This example will print, among other information, the CHIP ID and TROPIC01's firmware versions.

To flash this example to the Nucleo board on Linux, you can use a provided `flash.sh` script:

```sh
./flash.sh build/lt_ex_show_chip_id_and_fwver.elf
```

On Windows and other platforms you have to invoke OpenOCD manually.

Save the output of this example for future reference.

### Update Internal Firmware
After trying out communication and noting CHIP ID and firmware versions using the first example, upgrade TROPIC01's internal firmware, as newer versions fix bugs and ensure compatibility with the latest Libtropic SDK.

> [!IMPORTANT]
> - Using outdated firmware is not recommended. Outdated firmware may not be compatible with the latest version of the Libtropic SDK.
> - Firmware updates should be performed only after you saved output from `lt_ex_show_chip_id_and_fwver`.
> - Use a stable power source and avoid disconnecting the devkit or rebooting your computer during the update. Interrupting a firmware update can brick the device.

To update both internal firmware components to the latest versions, execute the following example:
```bash
./flash.sh build/lt_ex_fw_update.elf
```

After successful execution, your chip will contain the latest firmware and will be compatible with the current Libtropic API.

## Running Advanced Examples

> [!IMPORTANT]
> Make sure you already ran basic examples as described in previous section.

> [!CAUTION]
> Some examples cause **irreversible changes** to the chip. Proceed only after reading the [Examples](https://tropicsquare.github.io/libtropic/latest/get_started/examples/) section in the Libtropic documentation and you understand the consequences. The documentation describes which examples are irreversible and what each example does.

Other examples can be run using the same script as the examples above:

```bash
./flash.sh <path_to_ELF>
```

> [!IMPORTANT]
> You may encounter issues with examples that establish a Secure Session — refer to [Establishing Your First Secure Channel Session](https://tropicsquare.github.io/libtropic/latest/get_started/default_pairing_keys/#establishing-your-first-secure-channel-session) in the Libtropic documentation for more information.

## Building Functional Tests

> [!CAUTION]
> **DANGER!** Functional tests are for internal use only and are provided only for reference. Some tests can **destroy** your chip. **Do not run the tests** unless you are absolutely sure what you are doing. If you damage your chip with the tests, we are unable to provide any support.

All functional tests are implemented in the [Libtropic](https://github.com/tropicsquare/libtropic) repository. For more information about them, refer to the [Functional Tests](https://tropicsquare.github.io/libtropic/latest/for_contributors/functional_tests/) section in the Libtropic documentation.

Build all functional tests in one place with the following commands:
```bash
mkdir build
cd build
cmake -DLT_BUILD_TESTS=1 -DLT_CAL=mbedtls_v4 ..
make
```

For each test, a binary will be created in the build directory (similar to when building the examples).

> [!IMPORTANT]
> You may encounter issues with tests that establish a Secure Session — refer to [Establishing Your First Secure Channel Session](https://tropicsquare.github.io/libtropic/latest/get_started/default_pairing_keys/#establishing-your-first-secure-channel-session) in the Libtropic documentation for more information.

We use CTest to run functional tests.

To list available tests:
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
CTest uses the `run_test.sh` script, which flashes a binary containing the test and immediately reads the serial port output to evaluate the test results. This script attempts to automatically detect your NUCLEO board's serial port. However, if you are using a non-standard setup (e.g., a different ST-Link, special OS configuration) or have multiple NUCLEOs connected to your computer, you must manually specify the path to the serial port during CTest configuration:

```bash
cmake -DLT_BUILD_TESTS=1 -DLT_CAL=trezor_crypto -DSTLINK_UART=<path> ..
```

## FAQ

If you encounter any issues, please check the [FAQ](./../FAQ.md) before filing an issue or reaching out to our [support](https://support.desk.tropicsquare.com/).

> [!IMPORTANT]  
> We do not provide any support for running the tests.  