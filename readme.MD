# libtropic-stm32

## About

This repository contains examples of libtropic's usage on ST Microelectronic's development boards.

Contributors, please follow [guidelines](https://github.com/tropicsquare/libtropic-stm32/blob/master/CONTRIBUTING.md).

Currently supported boards are:

* NUCLEO-L432KC https://os.mbed.com/platforms/ST-Nucleo-L432KC/
* NUCLEO-F439ZI https://os.mbed.com/platforms/ST-Nucleo-F439ZI/


Debug messages are transmitted over devboard's USB ACM serial port, they can be received by opening a serial port on PC

More info about wiring is in each project's own README.md file.


## Dependencies

* cmake
* arm-none-eabi-gcc
* openocd

## Cloning

This repository must be cloned recursively, because it contains submodules - libtropic and target drivers:

```bash
git clone https://github.com/tropicsquare/libtropic-stm32.git
cd libtropic-stm32
git submodule update --init --recursive
```

## Compiling and Running Examples

To compile examples, pass `-DLT_BUILD_EXAMPLES=1` to CMake and a switch for each example to include. For example, to include only `lt_ex_hello_world()`, invoke CMake like this:
```bash
cmake -DLT_BUILD_EXAMPLES=1 -DLT_EX_HELLO_WORLD=1 ..
```
> Examples require SH0 private key to establish a secure session with TROPIC01. CMake variable `LT_SH0_PRIV_PATH` is used for that and its default value is set to path to the SH0 private key from the currently used `lab_batch_package`, found in `libtropic/provisioning_data/<lab_batch_package_directory>/sh0_key_pair/`. Naturally, it can be overridden with another path to SH0 private key.

All available examples are [here](https://github.com/tropicsquare/libtropic/tree/master/examples).

For a quick check of correct setup, the Hello World example is already compiled for all supported boards [here](https://github.com/tropicsquare/libtropic-stm32/actions/workflows/hello_world_artifact.yml) (click on latest workflow run, scroll down to `Artifacts` and download an artifact for your board).

All steps to build the Hello World example on the F439ZI:
```bash
cd NUCLEO_F439ZI/
mkdir build
cd build
cmake -DLT_BUILD_EXAMPLES=1 -DLT_EX_HELLO_WORLD=1 ..
make
```

### Flashing

Connect NUCLEO devboard with usb, then execute:

```bash
cd build/
make flash
```

Similar output should be visible over serialport:
```
52	;INFO;	=======================================================================
53	;INFO;	=====  TROPIC01 Hello World                                         ===
54	;INFO;	=======================================================================
57	;INFO;	-------------------------------------------------------------------------------------------------------------
58	;INFO;	 Session with H0 keys:
33	;INFO;Establish session with H0
35	;SYSTEM;ASSERT_OK
40	;INFO;lt_ping()
41	;SYSTEM;ASSERT_OK
42	;INFO;		Message: This is Hello World message from TROPIC01!!
63	;INFO;	-------------------------------------------------------------------------------------------------------------
65	;INFO;	 End of execution, no errors.

```

## Building Functional Tests
To build functional test suite from libtropic, pass `-DLT_BUILD_TESTS=1` to CMake:
```bash
cd NUCLEO_F439ZI/
mkdir build
cd build
cmake -DLT_BUILD_TESTS=1 ..
make
```

> Functional tests require SH0 private key to establish a secure session with TROPIC01. CMake variable `LT_SH0_PRIV_PATH` is used for that and its default value is set to path to the SH0 private key from the currently used `lab_batch_package`, found in `libtropic/provisioning_data/<lab_batch_package_directory>/sh0_key_pair/`. Naturally, it can be overridden with another path to SH0 private key.

For each test, an ELF binary will be created in the build directory.

Normally, we use CTest for handling the functional tests. They are executed using `lt_test_runner` on internal TS11 evaluation boards.

To launch all tests:
```bash
ctest
```
*Tip: To see all output use `--verbose`.*

To launch a selected test:
```bash
ctest -R test_name
```

To enumerate tests:
```bash
ctest -N
```

If you want to launch the tests without the platform, simply flash select binary manually and inspect
the UART output. Note that a different UART is used for test output. This method is not officially supported.

## Debugging in VS Code

For compiling in debug mode pass `-DCMAKE_BUILD_TYPE=Debug` during cmake call.

Install dependencies:

* cortex-debug extension
* arm-none-eabi-gdb

Then place this launch.json file into .vscode folder:

```
{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "cortex-debug",
            "request": "attach",
            "name": "Debug (OpenOCD)",
            "servertype": "openocd",
            "cwd": "${workspaceRoot}",
            //"preLaunchTask": "cargo build",
            //"runToEntryPoint": "true",
            "executable": "./build/stm32_example.elf",
            "device": "STM32F429ZI",
            "configFiles": [
                "/usr/share/openocd/scripts/board/stm32f429discovery.cfg"
            ],
        }
    ]
}

```

Debugging should be enabled after restarting your VS Code.

## Opening in STM32CubeIDE

* Open `File>New>STM32 Cmake Project`
* Set 'Project with existing CMake sources'
* Choose apropriate name, set source directory to `PATH_TO_THIS_EXAMPLE/stm32_example` and set the name of build directory
* Define used MCU (Supported device is `STM32F439ZITx and STM32L432KC`)
* Click on finish
* Define transport layer by uncommenting an option in CMakeLists.txt
* Click on Build, then on the left side click on `binaries`, right click on `stm32_example.elf` and choose 'run as a stm32 Application'

## License

See the [LICENSE.md](LICENSE.md) file in the root of this repository or consult license information at [Tropic Square website](https://tropicsquare.com/license).