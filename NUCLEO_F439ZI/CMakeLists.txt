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

if (NOT DEFINED LT_BUILD_EXAMPLES AND NOT DEFINED LT_BUILD_TESTS)
    message(FATAL_ERROR "Please define -DLT_BUILD_EXAMPLES=1 to build examples (and also select required example) or -DLT_BUILD_TESTS=1 to build tests. See docs. Nothing to do now, bye!")
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

# Add this so HAL knows what target MCU we use
add_definitions(-D${STM32_DEVICE})

###########################################################################
#                                                                         #
#   Add libtropic                                                         #
#                                                                         #
###########################################################################

# Libtropic related setup, use trezor crypto library for host side cryptography
set(LT_USE_TREZOR_CRYPTO ON)

# Provide path to a root of libtropic repository
add_subdirectory(${PATH_TO_LIBTROPIC} "libtropic")


###########################################################################
#                                                                         #
#   EXAMPLES CONFIGURATION                                                #
#                                                                         #
###########################################################################

if (LT_BUILD_EXAMPLES)
    # -------------------------------------------------------------------------
    # Add executable for examples
    # -------------------------------------------------------------------------
    add_executable(stm32_example.elf ${SOURCES} ${HEADERS})

    # -------------------------------------------------------------------------
    # Set on which USART pins will be output routed
    # -------------------------------------------------------------------------

    # Used for testing. Do not use it on when running on standalone Nucleo board
    if (LT_TESTING_RIG)
        target_compile_definitions(stm32_example.elf PRIVATE LT_TESTING_RIG)
    endif()

    #target_link_options(stm32_example.elf PRIVATE -Wl,--gc-sections -Wl,--strip-all)  # Use this to make compiled size smaller
    target_link_libraries(stm32_example.elf PRIVATE tropic)


    # -------------------------------------------------------------------------
    # Example selection
    #                                                                    
    # Define which examples will be compiled into main().              
    # If any of examples or test routines is enabled, libtropic will be
    # compiled with all examples exposed (controlled by libtropic's    
    # LT_ADD_EXAMPLES), then a concrete example or test routine will be
    # enabled in main() int this project.                              
    #                                                                    
    # -------------------------------------------------------------------------

    # Add examples
    set(LT_ADD_EXAMPLES ON)

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

    # -------------------------------------------------------------------------
    # Compilation outputs for examples
    # -------------------------------------------------------------------------

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
endif()

###########################################################################
#                                                                         #
# FUNCTIONAL TESTS CONFIGURATION                                          #
#                                                                         #
# This section will automatically configure CTest for launching tests     #
# defined in libtropic. Do NOT hardcode any test definitions here.        #
# Define them in libtropic.                                               #
#                                                                         #
# To build functional tests, use -DLT_BUILD_TESTS=1 in cmake invocation.  #
#                                                                         #
###########################################################################

if(LT_BUILD_TESTS)
    # Enable CTest.
    enable_testing()
    
    # So we can include preprocessed test registry (lt_test_registry.c.inc).
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/libtropic)

    # Loop through tests defined in libtropic and prepare environment.
    foreach(test_name IN LISTS LIBTROPIC_TEST_LIST)
        
        # Create a correct macro from test name.
        string(TOUPPER ${test_name} test_macro)
        string(REPLACE " " "_" test_macro ${test_macro})

        set(exe_name ${test_name}.elf)
        # We don't need binary format for now.
        # set(bin_name ${test_name}.bin)

        # Define executable (separate for each test) and link dependencies.
        add_executable(${exe_name} ${SOURCES} ${HEADERS})
        target_link_libraries(${exe_name} PRIVATE tropic)

        # Enable test registry using LT_BUILD_TESTS and choose correct test for the binary.
        target_compile_definitions(${exe_name} PRIVATE LT_BUILD_TESTS)
        target_compile_definitions(${exe_name} PRIVATE ${test_macro})

        # This is needed for our target to define correct UART output -- the test rig is connected
        # to an external UART.
        target_compile_definitions(${exe_name} PRIVATE LT_TESTING_RIG)

        # We don't need binary format for now.
        # add_custom_command(TARGET ${exe_name}
        #     POST_BUILD
        #     COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${exe_name}> ${bin_name}
        #     BYPRODUCTS ${bin_name}
        #     COMMENT "Converting ${exe_name} to ${bin_name}"
        # )

        # Add CTest entry.
        add_test(NAME ${test_name}
                COMMAND python3 -m lt_test_runner stm32_f439zi ${CMAKE_CURRENT_BINARY_DIR}/${exe_name}
        )
        # Correctly define Python module path, so we can call lt_test_runner.
        get_filename_component(ABSOLUTE_PATH_TO_LIBTROPIC ${PATH_TO_LIBTROPIC} ABSOLUTE)
        set_tests_properties(${test_name} PROPERTIES
            ENVIRONMENT "PYTHONPATH=${ABSOLUTE_PATH_TO_LIBTROPIC}/scripts/test_runner"
        )
    endforeach()
endif()

