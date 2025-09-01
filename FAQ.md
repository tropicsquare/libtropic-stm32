# FAQ

This list might help you to resolve some issues.

- [Cannot establish a secure session?](#cannot-establish-a-secure-session)
- [Debugging in VS Code](#debugging-in-vs-code)
- [Opening in STM32CubeIDE](#opening-in-stm32cubeide)


## Cannot establish a secure session?

Maybe you have TROPIC01 engineering sample - some of older engineering samples require different SH0 key. Try to compile with `-DLT_SH0_PRIV_PATH=../libtropic/provisioning_data/sh0_priv_engineering_sample01.pem`, for example like this:
```bash
cd NUCLEO_F439ZI/
mkdir build &&  cd build && cmake -DLT_BUILD_EXAMPLES=1 -DLT_SH0_PRIV_PATH=../libtropic/provisioning_data/sh0_priv_engineering_sample01.pem .. && make
```

## Debugging in VS Code

For compiling in debug mode, pass `-DCMAKE_BUILD_TYPE=Debug` during cmake call.

Install dependencies:

* cortex-debug extension
* arm-none-eabi-gdb

Then place this `launch.json` file into `.vscode` folder and update:
* `EXAMPLE_NAME` according to the one you programmed into board
* `configFiles` according to board you use

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
            "executable": "./build/EXAMPLE_NAME.elf",
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
