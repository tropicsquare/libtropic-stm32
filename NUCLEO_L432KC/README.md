# Nucleo L432KC + TROPIC01 Arduino shield

 > [!WARNING]
 > L432KC example is no longer updated and it stays here only for historical reasons

This folder contains examples of libtropic's usage on [Nucleo L432KC](https://os.mbed.com/platforms/ST-Nucleo-L432KC/) ST Microelectronic's development board.

Contributors, please follow [guidelines](https://github.com/tropicsquare/libtropic-stm32/blob/master/CONTRIBUTING.md).

We don't provide shield for this board, example expects TROPIC01 to be wired to the following pins:
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

## Clone

This repository must be cloned recursively, because it contains submodules with libtropic SDK and STM32's drivers.

```bash
git clone https://github.com/tropicsquare/libtropic-stm32.git
cd libtropic-stm32
git submodule update --init --recursive
cd NUCLEO_L432KC/
```

## Dependencies and Requirements

You need to have:
* cmake (Could be installed with `sudo apt install cmake`)
* arm-none-eabi-gcc (Could be installed with `sudo apt install gcc-arm-none-eabi`)
* openocd (https://openocd.org/pages/getting-openocd.html)

# Build All Examples

All examples are implemented in the [libtropic](https://github.com/tropicsquare/libtropic) repository. For more information about them, refer to the [Examples](https://tropicsquare.github.io/libtropic/latest/get_started/examples/) section in the libtropic documentation.

Build all examples in one place with following commands:

```bash
mkdir build
cd build
cmake -DLT_BUILD_EXAMPLES=1 ..
make
```
For each example, an ELF binary will be created in the build directory. Once all examples are built, continue with following chapter.

# Recommended First Steps

The Nucleo board provides a virtual serial port over USB. To view the output from the examples, connect to this serial port using a terminal emulator (e.g., `minicom`, `screen`, or PuTTY).

 > [!IMPORTANT]
 > Before you start with various examples, we strongly recommend to do two things first:
 > * Read CHIP ID and TROPIC01's firmware versions and **save printed output for future reference**
 > * Update TROPIC01's both internal firmware to latest version.

 > [!TIP]
 > If you want to do a quick connectivity check for example with 'Hello World' example, all elf binaries are available [here](https://github.com/tropicsquare/libtropic-stm32/actions/workflows/build_and_upload_examples.yml) (click on latest workflow run, scroll down to `Artifacts` and download an artifact for your board).

## Display And Save Chip ID And Fw Versions
To display actual versions of internal firmwares and details from CHIP ID data field, program and execute following example:
```bash
./flash.sh build/lt_ex_show_chip_id_and_fwver.elf
```

We recommend to save printed output for future reference.

## Update Internal Firmwares

To update both internal firmwares to latest versions program and execute following example:
```bash
./flash.sh build/lt_ex_fw_update.elf
```

After successfull execution your chip will contain latest firmwares and will be compatible with libtropic API.

## Building and Running Other Examples

> [!WARNING]
> Some examples cause irreversible changes to the chip. For more details, refer to the [Examples](https://tropicsquare.github.io/libtropic/latest/get_started/examples/) section in the libtropic documentation.


For each built example, a binary was created in the build directory. For example, flash and execute the `lt_ex_hello_world` example as:
```bash
./flash.sh build/lt_ex_hello_world.elf
```
In your serial port you should see some output similar to this:
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
> During build, SH0 keypair is automatically chosen from  `libtropic/provisioning_data/<lab_batch_package_directory>/sh0_key_pair/`, this SH0 key is present in the majority of distributed TROPIC01 chips. In certain cases (first engineering samples) it might be necessary to manually set it (in PEM or DER format) with following cmake switch: `-DLT_SH0_PRIV_PATH=../../libtropic/provisioning_data/sh0_priv_engineering_sample01.pem`


## Building Functional Tests

All functional tests are implemented in the [libtropic](https://github.com/tropicsquare/libtropic) repository. For more information about them, refer to the [Functional Tests](https://tropicsquare.github.io/libtropic/latest/for_contributors/functional_tests/) section in the libtropic documentation.

> [!WARNING]
> Some tests make irreversible changes to the chip, such as writing pairing keys. Those irreversible tests contain `_ire_` in their name. On the other hand, reversible tests are marked `_rev_` and are generally safe to run, as they only make temporary changes and always perform cleanup.

Build all functional tests in one place with the following commands:
```bash
mkdir build
cd build
cmake -DLT_BUILD_TESTS=1 ..
make
```

For each test, a binary will be created in the build directory (similarly as when building the examples).

> [!IMPORTANT]
> During build, SH0 keypair is automatically chosen from  `libtropic/provisioning_data/<lab_batch_package_directory>/sh0_key_pair/`, this SH0 key is present in the majority of distributed TROPIC01 chips. In certain cases (first engineering samples) it might be necessary to manually set it (in PEM or DER format) with following cmake switch: `-DLT_SH0_PRIV_PATH=../../libtropic/provisioning_data/sh0_priv_engineering_sample01.pem`


We recommend to use CTest for handling of functional tests.

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
> To see all output use `--verbose`.


# FAQ

If you encounter any issue, please have a look [here](./../FAQ.md) before filling an issue here or before reaching out to our [support](https://support.desk.tropicsquare.com/).

