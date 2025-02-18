cmake_minimum_required(VERSION 3.21.0)


###########################################################################
#                                                                         #
#   Paths and setup                                                       #
#                                                                         #
###########################################################################

# Specify stm32 device
set(STM32_DEVICE "STM32L432xx")
# Openocd config file for used nucleo board
set(OPENOCD_CFG "/usr/share/openocd/scripts/board/st_nucleo_l4.cfg")
# Path to linker script file
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Src/STM32L432KCUX_FLASH.ld)
# Path to toolchain file
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/toolchain.cmake)
# Paths to stm32 drivers
set(PATH_TO_STM32CUBEL4 "STM32CubeL4/Drivers" )
# Path to libtropic
set(PATH_TO_LIBTROPIC "../../../../")


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
    Src/stm32l4xx_it.c
    Src/stm32l4xx_hal_msp.c
    Src/system_stm32l4xx.c
    Src/startup_stm32l432kcux.s
    Src/syscalls.c
    Src/sysmem.c
    ${PATH_TO_LIBTROPIC}/hal/port/stm32/lt_port_stm32_nucleo_l432kc.c
    ${PATH_TO_STM32CUBEL4}/BSP/STM32L4xx_Nucleo_32/stm32l4xx_nucleo_32.c
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng.c
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng.c
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
)

set(HAL_HEADERS
    ${PATH_TO_STM32CUBEL4}/BSP/STM32L4xx_Nucleo_32/stm32l4xx_nucleo_32.h
    ${PATH_TO_STM32CUBEL4}/CMSIS/Device/ST/STM32L4xx/Include/stm32l4xx.h
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal_cortex.h
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal.h
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal_rcc.h
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal_pwr.h
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal_pwr_ex.h
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal_exti.h
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal_gpio.h
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal_spi.h
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal_uart.h
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/stm32l4xx_hal_rcc.h
)

set(HEADERS
    Inc/main.h
    Inc/stm32l4xx_hal_conf.h
    Inc/stm32l4xx_it.h
    ${HAL_HEADERS}
)

# Include path for directories containing header files
include_directories(
    Inc/
    ${PATH_TO_STM32CUBEL4}/BSP/STM32L4xx_Nucleo_32/
    ${PATH_TO_STM32CUBEL4}/CMSIS/Device/ST/STM32L4xx/Include/
    ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/
    ${PATH_TO_STM32CUBEL4}/CMSIS/Core/Include/
)


###########################################################################
#                                                                         #
#   Add executable and set definitions                                    #
#                                                                         #
###########################################################################

add_executable(stm32_example.elf ${SOURCES} ${HEADERS})

# Add this so HAL knows what target MCU we use
add_definitions(-D${STM32_DEVICE})

if(LT_USE_TREZOR_CRYPTO)
    target_compile_definitions(stm32_example.elf PRIVATE LT_USE_TREZOR_CRYPTO)
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

## Compile test routines:
# -DLT_EX_TEST_REVERSIBLE=1
if(LT_EX_TEST_REVERSIBLE)
    # This line makes libtropic to be compiled with exposed example functions
    set(LT_ADD_EXAMPLES ON)
    # This line will enable lt_test_reversible() test routine in final executable
    target_compile_definitions(stm32_example.elf PRIVATE LT_EX_TEST_REVERSIBLE)
endif()
# --DLT_EX_TEST_IREVERSIBLE=1
if(LT_EX_TEST_IREVERSIBLE)
    # This line makes libtropic to be compiled with exposed example functions
    set(LT_ADD_EXAMPLES ON)
    # This line will enable lt_test_ireversible() test routine in final executable
    target_compile_definitions(stm32_example.elf PRIVATE LT_EX_TEST_IREVERSIBLE)
endif()
# Compile examples:
# -DLT_EX_FW_UPDATE=1
if(LT_EX_FW_UPDATE)
    # This line makes libtropic to be compiled with exposed example functions
    set(LT_ADD_EXAMPLES ONSTM32L4xx)
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
