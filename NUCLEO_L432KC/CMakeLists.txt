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
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32L432KCUX_FLASH.ld)
# Path to toolchain file
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../toolchain.cmake)
# Paths to stm32 drivers (added as a submodule)
set(PATH_TO_STM32CUBEL4 "Vendor/STM32CubeL4/Drivers" )
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
#   Vendor Libraries                                                      #
#                                                                         #
###########################################################################

# Create a library for all STM32CubeL4 HAL source files
add_library(stm32_vendor STATIC
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

# Tell CMake to treat the HAL include directories as SYSTEM directories
target_include_directories(stm32_vendor PUBLIC
    SYSTEM ${PATH_TO_STM32CUBEL4}/CMSIS/Core/Include/
    SYSTEM ${PATH_TO_STM32CUBEL4}/CMSIS/Device/ST/STM32L4xx/Include/
    SYSTEM ${PATH_TO_STM32CUBEL4}/BSP/STM32L4xx_Nucleo_32/
    SYSTEM ${PATH_TO_STM32CUBEL4}/STM32L4xx_HAL_Driver/Inc/
)

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
    ${PATH_TO_LIBTROPIC}/hal/port/stm32
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

    # So we can include preprocessed example registry (lt_ex_registry.c.inc).
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/libtropic)

    # Loop through examples defined in libtropic and prepare environment.
    foreach(example_name IN LISTS LIBTROPIC_EXAMPLE_LIST)

        # Create a correct macro from example name.
        string(TOUPPER ${example_name} example_macro)
        string(REPLACE " " "_" example_macro ${example_macro})

        set(exe_name ${example_name}.elf)

        # Define executable (separate for each example) and link dependencies.
        add_executable(${exe_name} ${SOURCES} ${HEADERS})
        target_link_libraries(${exe_name} PRIVATE tropic stm32_vendor)

        # Apply the strict compilation flags
        # (they are already applied for libtropic from the libtropic's CMakeLists.txt)
        if(LT_STRICT_COMP_FLAGS)
            target_link_libraries(${exe_name} PRIVATE libtropic::strict_comp_flags)
        endif()

        # Enable example registry using LT_BUILD_EXAMPLES and choose correct example for the binary.
        target_compile_definitions(${exe_name} PRIVATE LT_BUILD_EXAMPLES)
        target_compile_definitions(${exe_name} PRIVATE ${example_macro})

        # Used for testing. Do not use it on when running on standalone Nucleo board
        if (LT_TESTING_RIG)
            target_compile_definitions(${exe_name} PRIVATE LT_TESTING_RIG)
        endif()

    endforeach()

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
        target_link_libraries(${exe_name} PRIVATE tropic stm32_vendor)

        # Apply the strict compilation flags
        # (they are already applied for libtropic from the libtropic's CMakeLists.txt)
        if(LT_STRICT_COMP_FLAGS)
            target_link_libraries(${exe_name} PRIVATE libtropic::strict_comp_flags)
        endif()

        # Enable test registry using LT_BUILD_TESTS and choose correct test for the binary.
        target_compile_definitions(${exe_name} PRIVATE LT_BUILD_TESTS)
        target_compile_definitions(${exe_name} PRIVATE ${test_macro})

        # Used for testing. Do not use it on when running on standalone Nucleo board
        if (LT_TESTING_RIG)
            target_compile_definitions(${exe_name} PRIVATE LT_TESTING_RIG)
        endif()

        # We don't need binary format for now.
        # add_custom_command(TARGET ${exe_name}
        #     POST_BUILD
        #     COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${exe_name}> ${bin_name}
        #     BYPRODUCTS ${bin_name}
        #     COMMENT "Converting ${exe_name} to ${bin_name}"
        # )

        # Add CTest entry.
        add_test(NAME ${test_name}
                COMMAND python3 -m lt_test_runner stm32_l432kc ${CMAKE_CURRENT_BINARY_DIR}/${exe_name}
        )
        # Correctly define Python module path, so we can call lt_test_runner.
        get_filename_component(ABSOLUTE_PATH_TO_LIBTROPIC ${PATH_TO_LIBTROPIC} ABSOLUTE)
        set_tests_properties(${test_name} PROPERTIES
            ENVIRONMENT "PYTHONPATH=${ABSOLUTE_PATH_TO_LIBTROPIC}/scripts/test_runner"
        )
    endforeach()
endif()