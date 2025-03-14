# About

This repository contains examples of libtropic's usage on ST Microelectronic's development boards. Currently supported are:

* NUCLEO-L432KC https://os.mbed.com/platforms/ST-Nucleo-L432KC/
* NUCLEO-F439ZI https://os.mbed.com/platforms/ST-Nucleo-F439ZI/


Debug messages are transmitted over devboard's USB ACM serial port, they can be received by opening a serial port on PC

More info about wiring is in each project's own README.md file.


## Dependencies

* cmake
* arm-none-eabi-gcc
* openocd

## Cloning

This repository must be cloned recursively, because it contains submodules with drivers:

```bash
git clone https://github.com/tropicsquare/libtropic-stm32.git
cd libtropic-stm32
git submodule update --init --recursive
```
Also this repository is designed to work together with main libtropic repository. Make sure that examples' CMakeLists file contains correctly defined `PATH_TO_LIBTROPIC` which points to libtropic repository.


## Compiling

Based on compilation switches passed to cmake, particular example will be compiled in. For example when `-DLT_EX_HELLO_WORLD=1` is used, function `lt_ex_hello_world()` will be executed in main. All available examples are [here](https://github.com/tropicsquare/libtropic/tree/master/examples).

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

Debugging should be enabled after restarting your VS Code.

# Opening in STM32CubeIDE

* Open `File>New>STM32 Cmake Project`
* Set 'Project with existing CMake sources'
* Choose apropriate name, set source directory to `PATH_TO_THIS_EXAMPLE/stm32_example` and set the name of build directory
* Define used MCU (Supported device is `STM32F439ZITx and STM32L432KC`)
* Click on finish
* Define transport layer by uncommenting an option in CMakeLists.txt
* Click on Build, then on the left side click on `binaries`, right click on `stm32_example.elf` and choose 'run as a stm32 Application'

# License

See the [LICENSE.md](LICENSE.md) file in the root of this repository or consult license information at [Tropic Square website](https://tropicsquare.com/license).