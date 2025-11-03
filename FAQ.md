# FAQ

This list might help you resolve some issues.

- [FAQ](#faq)
  - [I cannot establish a secure session](#i-cannot-establish-a-secure-session)
  - [How to debug in VS Code?](#how-to-debug-in-vs-code)
  - [How to open the project in STM32CubeIDE?](#how-to-open-the-project-in-stm32cubeide)

## I cannot establish a secure session

You might be using wrong default pairing keys. Refer to [Default Pairing Keys for a Secure Channel Handshake](https://tropicsquare.github.io/libtropic/latest/get_started/default_pairing_keys/) section in libtropic documentation for more information.

## How to debug in VS Code?

To compile in debug mode, pass `-DCMAKE_BUILD_TYPE=Debug` during the CMake call.

Install the following dependencies:

* `cortex-debug` extension
* `arm-none-eabi-gdb`

Then place this `launch.json` file into the `.vscode` folder and update:

* `EXAMPLE_NAME` according to the example you programmed into the board.
* `configFiles` according to the board you are using.

```json
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
            ]
        }
    ]
}
```

Debugging should be enabled after restarting VS Code.

## How to open the project in STM32CubeIDE?

* Open `File > New > STM32 CMake Project`.
* Set the project type to 'Project with existing CMake sources'.
* Choose an appropriate name, set the source directory to `PATH_TO_THIS_EXAMPLE/<binary_name>`, and specify the build directory.
* Define the MCU being used (supported devices are `STM32F439ZITx` and `STM32L432KC`).
* Click on Finish.
* Define the transport layer by uncommenting an option in `CMakeLists.txt`.
* Click on Build. Then, on the left side, click on `binaries`, right-click on one of the built binaries, and choose 'Run as STM32 Application'.

