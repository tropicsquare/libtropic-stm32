# Nucleo L432KC Tutorial
This folder contains examples demonstrating how to use Libtropic on the [Nucleo L432KC](https://os.mbed.com/platforms/ST-Nucleo-L432KC/) STMicroelectronics development board. The examples assume the TROPIC01 Arduino shield or an equivalent wiring to the pins listed below.

**Note:** We do not ship an Arduino shield for this board; if you're using a custom PCB or MikroE's Secure Tropic Click, wire TROPIC01 accordingly (table below).

```
|------------|-----------------|-----------------|
|  TROPIC01  |  STM32L432KCU6  |  NUCLEO L432KC  |
|------------|-----------------|-----------------|
|  GND       |  GND            |  GND            |
|  3V3       |  3V3            |  3V3            |
|  MISO      |  GPIOA_PIN_6    |  A6             |
|  MOSI      |  GPIOA_PIN_7    |  A5             |
|  SCK       |  GPIOA_PIN_5    |  A4             |
|  CS        |  GPIOA_PIN_4    |  A3             |
|------------|-----------------|-----------------|
```

![NUCLEO L432KC wiring](img/nucleo_L432KC_connection_to_tropic.jpg)

Contents:

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
> For this tutorial, we recommend using a Linux machine if available.

### Install Dependencies
Make sure to have these dependencies installed:

* CMake
	* Ubuntu/Debian: `sudo apt install cmake`
	* Fedora: `sudo dnf install cmake`
	* Windows: [official web](https://cmake.org/download/)
* arm-none-eabi-gcc
	* Ubuntu/Debian: `sudo apt install gcc-arm-none-eabi`
	* Fedora: `sudo dnf install arm-none-eabi-gcc`
	* Windows: [official web](https://developer.arm.com/downloads/-/gnu-rm)
* OpenOCD (https://openocd.org/pages/getting-openocd.html)
	* Ubuntu/Debian: `sudo apt install openocd`
	* Fedora: `sudo dnf install openocd`
	* Windows: build from source or find pre-built binaries
* Serial monitor of your choice (`minicom`, `screen`, `GTKTerm`, `PuTTY`, or `RealTerm`)

MbedTLS 4.0.0 used in this repository requires:

* Recent Python
* The following Python packages:
	* jinja2
	* jsonschema

Recommended (Linux) setup with a Python virtual environment:
```shell
python3 -m venv .venv
source .venv/bin/activate
pip3 install --upgrade pip
pip3 install jinja2 jsonschema
```

### System Setup

#### GNU/Linux
To upload example programs, ensure you have access to USB devices (usually by being a member of the `plugdev` group) and to serial ports (usually the `dialout` group). Configure udev rules if required by OpenOCD.

```bash
# Check current groups
groups
# Add yourself to required groups if needed
sudo usermod -aG plugdev "$USER"
sudo usermod -aG dialout "$USER"
```

### Clone the libtropic-stm32 Repository

This repository must be cloned recursively because it contains submodules with dependencies (such as Libtropic and STM32 libraries).

```bash
git clone https://github.com/tropicsquare/libtropic-stm32.git
cd libtropic-stm32
git submodule update --init --recursive
cd NUCLEO_L432KC/
```

### Build Examples

All examples are implemented in the [Libtropic](https://github.com/tropicsquare/libtropic) repository. For more information, see the [Examples](https://tropicsquare.github.io/libtropic/latest/get_started/examples/) section in the Libtropic SDK documentation.

Build examples with the recommended configuration:

```bash
mkdir build
cd build
cmake -DLT_BUILD_EXAMPLES=1 -DLT_CAL=mbedtls_v4 ..
make
```

> [!NOTE]
> `-DLT_CAL` selects the Cryptographic Function Provider. We provide multiple providers — refer to the [Supported Cryptographic Functionality Providers](https://tropicsquare.github.io/libtropic/latest/other/supported_cfps/) section in the Libtropic SDK documentation.

For each example, an ELF binary will be created in the `build` directory. Once built, continue with the following section.

### Run a Basic Example: Read CHIP ID and firmware versions
The Nucleo board provides a virtual serial port over USB. To view example output, connect to this serial port using your preferred serial monitor.

First run the `lt_ex_show_chip_id_and_fwver` example — it prints the CHIP ID and TROPIC01 firmware versions. Flash it using the provided script on Linux:

```sh
./flash.sh build/lt_ex_show_chip_id_and_fwver.elf
```

On other platforms, invoke OpenOCD manually.

Save the output of this example for future reference.

> [!IMPORTANT]
> The `flash.sh` script attempts to autodiscover the ST-Link interface. If multiple ST-Links are connected, provide the ST-Link serial number as a second argument: `./flash.sh build/lt_ex_show_chip_id_and_fwver.elf <serial_number>`.

### Update Internal Firmware
After reading CHIP ID and firmware versions, update TROPIC01's internal firmware to ensure compatibility with the latest Libtropic SDK.

> [!IMPORTANT]
> - Using outdated firmware is not recommended.
> - Save output from `lt_ex_show_chip_id_and_fwver` before updating.
> - Use a stable power source and avoid disconnecting the board during an update.

To update both internal firmware components to the latest versions:
```bash
./flash.sh build/lt_ex_fw_update.elf
```

After successful execution your chip will contain the latest firmware and be compatible with the current Libtropic API.

## Running Advanced Examples

> [!IMPORTANT]
> Make sure you already ran the basic examples described above.

> [!CAUTION]
> Some examples cause **irreversible changes** to the chip. Read the [Examples](https://tropicsquare.github.io/libtropic/latest/get_started/examples/) section in the Libtropic documentation to learn which examples are irreversible and what they do.

Run other examples the same way:

```bash
./flash.sh <path_to_ELF>
```

> [!IMPORTANT]
> You may encounter issues with examples that establish a Secure Session — refer to [Establishing Your First Secure Channel Session](https://tropicsquare.github.io/libtropic/latest/get_started/default_pairing_keys/#establishing-your-first-secure-channel-session) in the Libtropic documentation.

## Building Functional Tests

> [!CAUTION]
> **DANGER!** Functional tests are for internal use only and are provided only for reference. Some tests can **destroy** your chip. **Do not run the tests** unless you are absolutely sure what you are doing.

All functional tests are implemented in the [Libtropic](https://github.com/tropicsquare/libtropic) repository. For more information, see the [Functional Tests](https://tropicsquare.github.io/libtropic/latest/for_contributors/functional_tests/) section.

Build functional tests with:
```bash
mkdir build
cd build
cmake -DLT_BUILD_TESTS=1 -DLT_CAL=mbedtls_v4 ..
make
```

For each test a binary will be created in the build directory.

We use CTest to run functional tests.

To list available tests:
```bash
ctest -N
```

To run reversible tests only:
```bash
ctest -R _rev_
```

> [!TIP]
> To see full output, use `--verbose` or `-V`.

### Implementation Details and Troubleshooting
`run_test.sh` flashes a binary and reads serial output to evaluate results. The script attempts to auto-detect your Nucleo's ST-Link. If multiple ST-Links are connected, specify the ST-Link serial number as a CMake parameter:

```bash
cmake -DLT_BUILD_TESTS=1 -DLT_CAL=mbedtls_v4 -DSTLINK_SERIAL_NUMBER=<serial_number> ..
```

If you use an external debugger or a special flashing setup (JTAG, external ST-Link), create your own flashing steps and do not rely on the provided scripts.

## FAQ
If you encounter any issues, please check the [FAQ](./../FAQ.md) before filing an issue or contacting our [support](https://support.desk.tropicsquare.com/).

> [!IMPORTANT]
> We do not provide support for running the tests.