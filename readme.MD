# libtropic-stm32

## About

This repository contains examples of libtropic's usage on ST Microelectronic's development boards.

Contributors, please follow [guidelines](https://github.com/tropicsquare/libtropic-stm32/blob/master/CONTRIBUTING.md).

Currently supported boards are:
* NUCLEO-L432KC https://os.mbed.com/platforms/ST-Nucleo-L432KC/
* NUCLEO-F439ZI https://os.mbed.com/platforms/ST-Nucleo-F439ZI/


Debug messages are transmitted over devboard's USB ACM serial port, they can be received by opening a serial port on PC.

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

## Building and Running Examples
All available examples can be found [here](https://github.com/tropicsquare/libtropic/tree/master/examples).

To build examples (in this case, for the `NUCLEO_F439ZI` board), do the following:
```bash
cd NUCLEO_F439ZI/
mkdir build
cd build
cmake -DLT_BUILD_EXAMPLES=1 ..
make
```

For each example, an ELF binary will be created in the build directory.

For a quick check of correct setup, all examples are already precompiled for all supported boards [here](https://github.com/tropicsquare/libtropic-stm32/actions/workflows/build_and_upload_examples.yml) (click on latest workflow run, scroll down to `Artifacts` and download an artifact for your board).

> [!IMPORTANT]
> When `LT_BUILD_EXAMPLES` are set, there has to be a way to define the SH0 private key for the TROPIC01's pairing key slot 0, because both the examples and the tests depend on it. For this purpose, the CMake variable `LT_SH0_PRIV_PATH` is used, which should hold the path to the file with the SH0 private key in PEM or DER format. By default, the path is set to the currently used lab batch package, found in `libtropic/provisioning_data/<lab_batch_package_directory>/sh0_key_pair/`. But it can be overriden by the user either from the command line when executing CMake (switch `-DLT_SH0_PRIV_PATH=<path>`), or from a child CMakeLists.txt.

### Flashing

Connect NUCLEO devboard with usb, then execute in the `NUCLEO_F439ZI/` directory:

```bash
./flash.sh build/<chosen_binary>
```

After this, output should be seen in the opened serial port.

## Building Functional Tests
All available functional tests can be found [here](https://github.com/tropicsquare/libtropic/tree/master/tests/functional/).

To build functional tests (in this case, for the `NUCLEO_F439ZI` board), do the following:
```bash
cd NUCLEO_F439ZI/
mkdir build
cd build
cmake -DLT_BUILD_TESTS=1 ..
make
```

For each test, an ELF binary will be created in the build directory.

> [!IMPORTANT]
> When `LT_BUILD_TESTS` are set, there has to be a way to define the SH0 private key for the TROPIC01's pairing key slot 0, because both the examples and the tests depend on it. For this purpose, the CMake variable `LT_SH0_PRIV_PATH` is used, which should hold the path to the file with the SH0 private key in PEM or DER format. By default, the path is set to the currently used lab batch package, found in `libtropic/provisioning_data/<lab_batch_package_directory>/sh0_key_pair/`. But it can be overriden by the user either from the command line when executing CMake (switch `-DLT_SH0_PRIV_PATH=<path>`), or from a child CMakeLists.txt.


Normally, we use CTest for handling the functional tests. They are executed using `lt_test_runner` on internal TS11 evaluation boards.

To launch all tests:
```bash
ctest
```
> [!TIP]
> To see all output use `--verbose`.

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

For compiling in debug mode, pass `-DCMAKE_BUILD_TYPE=Debug` during cmake call.

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
* Choose apropriate name, set source directory to `PATH_TO_THIS_EXAMPLE/<binary_name>` and set the name of build directory
* Define used MCU (Supported device is `STM32F439ZITx` and `STM32L432KC`)
* Click on finish
* Define transport layer by uncommenting an option in CMakeLists.txt
* Click on Build, then on the left side click on `binaries`, right click on one of the built binaries and choose 'run as a stm32 Application'

## License

See the [LICENSE.md](LICENSE.md) file in the root of this repository or consult license information at [Tropic Square website](https://tropicsquare.com/license).