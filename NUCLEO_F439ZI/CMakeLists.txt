cmake_minimum_required(VERSION 3.21.0)


###########################################################################
#                                                                         #
#   Paths and setup                                                       #
#                                                                         #
###########################################################################

# Specify stm32 device
set(STM32_DEVICE "STM32F439xx")
# Openocd config file for used nucleo board
set(OPENOCD_CFG "/usr/share/openocd/scripts/board/stm32f429discovery.cfg")
# Path to linker script file
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F429ZITX_FLASH.ld)
# Path to toolchain file
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../toolchain.cmake)
# Paths to stm32 drivers (added as a submodule)
set(PATH_TO_STM32CUBEF4 "Vendor/STM32CubeF4/Drivers" )
# Path to libtropic (added as a submodule)
if(NOT DEFINED PATH_TO_LIBTROPIC)
    set(PATH_TO_LIBTROPIC "../libtropic")
endif()


###########################################################################
#                                                                         #
#   Define project's name                                                 #
#                                                                         #
###########################################################################

project(stm32_example LANGUAGES C ASM)


###########################################################################
#                                                                         #
#   Sources                                                               #
#                                                                         #
###########################################################################

# Specify other source files
set(SOURCES ${SOURCES}
    Src/main.c
    Src/stm32f4xx_it.c
    Src/stm32f4xx_hal_msp.c
    Src/system_stm32f4xx.c
    Src/startup_stm32f429zitx.s
    Src/syscalls.c
    Src/sysmem.c
    ${PATH_TO_LIBTROPIC}/hal/port/stm32/lt_port_stm32_nucleo_f439zi.c
    ${PATH_TO_STM32CUBEF4}/BSP/STM32F4xx_Nucleo_144/stm32f4xx_nucleo_144.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
)

set(HEADERS
    ${PATH_TO_STM32CUBEF4}/BSP/STM32F4xx_Nucleo_144/stm32f4xx_nucleo_144.h
    ${PATH_TO_STM32CUBEF4}/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_cortex.h
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rcc.h
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_pwr.h
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_pwr_ex.h
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_exti.h
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_gpio.h
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_spi.h
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_uart.h
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rcc.h
)

set(HEADERS
    Inc/main.h
    Inc/stm32f4xx_hal_conf.h
    Inc/stm32f4xx_it.h
    ${HEADERS}
)

# Include path for directories containing header files
include_directories(
    Inc/
    ${PATH_TO_STM32CUBEF4}/CMSIS/Core/Include/
    ${PATH_TO_STM32CUBEF4}/CMSIS/Device/ST/STM32F4xx/Include/
    ${PATH_TO_STM32CUBEF4}/BSP/STM32F4xx_Nucleo_144/
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/
)


###########################################################################
#                                                                         #
#   Add executable and set definitions                                    #
#                                                                         #
###########################################################################

add_executable(stm32_example.elf ${SOURCES} ${HEADERS})

# Add this so HAL knows what target MCU we use
add_definitions(-D${STM32_DEVICE})


###########################################################################
#                                                                         #
#   Set on which USART pins will be output routed                         #
#                                                                         #
###########################################################################

# Used for testing. Do not use it on when running on standalone Nucleo board
if (LT_TESTING_RIG)
    target_compile_definitions(stm32_example.elf PRIVATE LT_TESTING_RIG)
endif()


###########################################################################
#                                                                         #
#   Define which examples will be compiled into main().                   #
#   If any of examples or test routines is enabled, libtropic will be     #
#   compiled with all examples exposed (controlled by libtropic's         #
#   LT_ADD_EXAMPLES), then a concrete example or test routine will be     #
#   enabled in main() int this project.                                   #
#                                                                         #
###########################################################################

# Following two lines should be placed in next section,
# but let's keep them here for clarity:
set(LT_ADD_EXAMPLES ON)
set(LT_ADD_FUNC_TESTS ON)

# Compile examples:
# -DLT_EX_FW_UPDATE=1
if(LT_EX_FW_UPDATE)
    # This line makes libtropic to be compiled with exposed example functions
    set(LT_ADD_EXAMPLES ON)
    set(LT_ENABLE_FW_UPDATE ON)
    # This line will enable lt_ex_fw_update() example in final executable
    target_compile_definitions(stm32_example.elf PRIVATE LT_EX_FW_UPDATE)
endif()
# -DLT_EX_HELLO_WORLD=1
if(LT_EX_HELLO_WORLD)
    # This line makes libtropic to be compiled with exposed example functions
    set(LT_ADD_EXAMPLES ON)
    # This line will enable lt_ex_hello_world() example in final executable
    target_compile_definitions(stm32_example.elf PRIVATE LT_EX_HELLO_WORLD)
endif()
# -DLT_EX_HW_WALLET=1
if(LT_EX_HW_WALLET)
    # This line makes libtropic to be compiled with exposed example functions
    set(LT_ADD_EXAMPLES ON)
    # This line will enable lt_ex_hw_wallet() example in final executable
    target_compile_definitions(stm32_example.elf PRIVATE LT_EX_HW_WALLET)
endif()


# Compile test routines:
# -DLT_TEST_CHIP_ID=1
if(LT_TEST_CHIP_ID)
    target_compile_definitions(stm32_example.elf PRIVATE LT_TEST_CHIP_ID)
endif()
# -DLT_TEST_WRITE_PAIRING_KEYS=1
if(LT_TEST_WRITE_PAIRING_KEYS)
    target_compile_definitions(stm32_example.elf PRIVATE LT_TEST_WRITE_PAIRING_KEYS)
endif()
# -DLT_TEST_WRITE_R_CONFIG=1
if(LT_TEST_WRITE_R_CONFIG)
    target_compile_definitions(stm32_example.elf PRIVATE LT_TEST_WRITE_R_CONFIG)
endif()
# --DLT_TEST_PING=1
if(LT_TEST_PING)
    target_compile_definitions(stm32_example.elf PRIVATE LT_TEST_PING)
endif()
# --DLT_TEST_ECC_EDDSA=1
if(LT_TEST_ECC_EDDSA)
    target_compile_definitions(stm32_example.elf PRIVATE LT_TEST_ECC_EDDSA)
endif()
# --DLT_TEST_R_MEM=1
if(LT_TEST_R_MEM)
    target_compile_definitions(stm32_example.elf PRIVATE LT_TEST_R_MEM)
endif()
# --DLT_TEST_ERASE_R_CONFIG=1
if(LT_TEST_ERASE_R_CONFIG)
    target_compile_definitions(stm32_example.elf PRIVATE LT_TEST_ERASE_R_CONFIG)
endif()

###########################################################################
#                                                                         #
#   Add libtropic                                                         #
#                                                                         #
###########################################################################

# Libtropic related setup, use trezor crypto library for host side cryptography
set(LT_USE_TREZOR_CRYPTO ON)

# Provide path to a root of libtropic repository
add_subdirectory(${PATH_TO_LIBTROPIC} "libtropic")

#target_link_options(stm32_example.elf PRIVATE -Wl,--gc-sections -Wl,--strip-all)  # Use this to make compiled size smaller
target_link_libraries(stm32_example.elf PRIVATE tropic)


###########################################################################
#                                                                         #
#   Compilation outputs                                                   #
#                                                                         #
###########################################################################

# Create a binary from the elf
add_custom_command(TARGET stm32_example.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -Oihex stm32_example.elf stm32_example.hex
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(TARGET stm32_example.elf
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary stm32_example.elf stm32_example.bin
)

add_custom_target(flash
    COMMAND openocd -f ${OPENOCD_CFG} -c 'program stm32_example.elf verify reset exit'
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS stm32_example.elf
)
