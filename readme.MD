# About

This repository contains examples of libtropic usage on ST Microelectronic's development boards.

Contributors, please follow [guidelines](https://github.com/tropicsquare/libtropic-stm32/blob/master/CONTRIBUTING.md).

Currently supported boards are:

* [NUCLEO-L432KC](https://os.mbed.com/platforms/ST-Nucleo-L432KC/)
* [NUCLEO-F439ZI](https://os.mbed.com/platforms/ST-Nucleo-F439ZI/)


Debug messages are transmitted over devboard USB ACM serial port, they can be received by opening a serial port on PC

More info about wiring is in each project's own README.md file.


## Dependencies

* cmake
* arm-none-eabi-gcc
* openocd

## Cloning

This repository must be cloned recursively. Repository contains submodules - libtropic and target drivers:

```bash
git clone https://github.com/tropicsquare/libtropic-stm32.git
cd libtropic-stm32
git submodule update --init --recursive
```

## Compiling

Based on compilation switches passed to cmake, particular example will be compiled in. Example; `-DLT_EX_HELLO_WORLD=1` is used, function `lt_ex_hello_world()` will be executed in main. All available examples are [here](https://github.com/tropicsquare/libtropic/tree/master/examples).

For a quick check of the correct setup, the Hello World example is already compiled for all supported boards [here](https://github.com/tropicsquare/libtropic-stm32/actions/workflows/hello_world_artifact.yml) (click the latest workflow run, scroll down to `Artifacts` and download an artifact for your board).

```bash
cd NUCLEO_L432KC/
mkdir build
cd build
cmake -DLT_EX_HELLO_WORLD=1 ..
make
```

## Flashing

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


# Debugging in VS Code

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

Restart your VS code to enable debugging.

# Opening in STM32CubeIDE

1. Open `File>New>STM32 Cmake Project`
2. Set 'Project with existing CMake sources'
3. Choose apropriate name 
4. Set source directory to `PATH_TO_THIS_EXAMPLE/stm32_example`
5. Set the name of the build directory
6. Define used MCU (Supported device is `STM32F439ZITx and STM32L432KC`)
7. Click finish
8. Define transport layer by uncommenting an option in CMakeLists.txt
9. Click Build
10. Click `binaries`
11. Right click `stm32_example.elf`
12. Choose 'run as a stm32 Application'

# License

See the [LICENSE.md](LICENSE.md) file in the root of this repository or consult license information at [Tropic Square website](https://tropicsquare.com/license).
