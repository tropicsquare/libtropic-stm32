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
    set(PATH_TO_LIBTROPIC "../vendor/libtropic/")
endif()

if (NOT DEFINED LT_BUILD_EXAMPLES AND NOT DEFINED LT_BUILD_TESTS)
    message(FATAL_ERROR "Please define -DLT_BUILD_EXAMPLES=1 to build examples (and also select required example) or -DLT_BUILD_TESTS=1 to build tests. See docs. Nothing to do now, bye!")
endif()

option(LT_USE_INT_PIN "Use INT pin instead of polling for TROPIC01's response" OFF)

# Path to STLink UART device which will be used for reading test output.
# You need to define this only if you:
#   1. Want to run tests using CTest AND
#   2. You are using standalone STM32 connected using built-in STLink AND
#   3. STLink UART cannot be discovered automatically using the CTest script.
if (NOT DEFINED STLINK_UART)
    set(STLINK_UART "")
endif()

# Optional prefix to tests registered to CTest. Useful when running same test against
# different chips to differentiate them by their name in JUnit output.
if (NOT DEFINED CTEST_PREFIX)
    set(CTEST_PREFIX "")
endif()

option(LT_TEST_RUNNER_DEBUG "Run lt_test_runner with debug output enabled." OFF)
if (LT_TEST_RUNNER_DEBUG)
    set(LT_TEST_RUNNER_FLAGS "-v")
else()
    set(LT_TEST_RUNNER_FLAGS "")
endif()

###########################################################################
#                                                                         #
#   Define project's name                                                 #
#                                                                         #
###########################################################################

project(stm32_example LANGUAGES C ASM)

###########################################################################
#                                                                         #
#   Add libtropic                                                         #
#                                                                         #
###########################################################################

# Libtropic related setup, use trezor crypto library for host side cryptography
set(LT_CRYPTO "trezor_crypto")

# Provide path to a root of libtropic repository
add_subdirectory(${PATH_TO_LIBTROPIC} "libtropic")

###########################################################################
#                                                                         #
#   Crypto backend handling                                               #
#                                                                         #
#   Note: The libtropic consumer does not have to do this in his          #
#   application, we do this to quickly switch crypto backends.            #
#   Refer to the libtropic documentation for more info about what should  #
#   be done by the consumer.                                              #
#                                                                         #
###########################################################################

# Developers who integrate Libtropic do not have to use these variables
# It's used in main.c to switch crypto contexts without manual changes
set(LT_USE_TREZOR_CRYPTO 0 CACHE INTERNAL "")
set(LT_USE_MBEDTLS_V4 0 CACHE INTERNAL "")

# Handle LT_CAL
if(LT_CAL STREQUAL "trezor_crypto")
    message(STATUS "Crypto provider set to trezor_crypto")
    add_subdirectory("${PATH_TO_LIBTROPIC}cal/trezor_crypto" "trezor_crypto_cal")
    
    # Developers who integrate Libtropic do not have to use this variable
    # It's used in main.c to switch crypto contexts without manual changes
    set(LT_USE_TREZOR_CRYPTO 1)

    add_subdirectory("${PATH_TO_LIBTROPIC}/vendor/trezor_crypto/" "trezor_crypto")
    target_compile_definitions(trezor_crypto PRIVATE AES_VAR USE_INSECURE_PRNG)
    target_link_libraries(tropic PUBLIC trezor_crypto)
elseif(LT_CAL STREQUAL "mbedtls_v4")
    message(STATUS "Crypto provider set to mbedtls_v4")
    add_subdirectory("${PATH_TO_LIBTROPIC}cal/mbedtls_v4" "mbedtls_v4_cal")

    # Developers who integrate Libtropic do not have to use this variable
    # It's used in main.c to switch crypto contexts without manual changes
    set(LT_USE_MBEDTLS_V4 1)

    set(ENABLE_TESTING OFF CACHE BOOL "Disable mbedtls_v4 test building.")
    set(ENABLE_PROGRAMS OFF CACHE BOOL "Disable mbedtls_v4 examples building.")
    add_subdirectory("${PATH_VENDOR}mbedtls_v4/" "mbedtls_v4")

    target_link_libraries(tropic PUBLIC mbedtls)
else()
    get_property(lt_cal_choices CACHE LT_CAL PROPERTY STRINGS)
    message(FATAL_ERROR "Incorrect CAL set to LT_CAL!\nSupported CALs: ${lt_cal_choices}")
endif()

target_sources(tropic PRIVATE ${LT_CAL_SRCS})
target_include_directories(tropic PUBLIC ${LT_CAL_INC_DIRS})

###########################################################################
#                                                                         #
#   Sources                                                               #
#                                                                         #
###########################################################################

# Specify other source files
set(SOURCES
    ${PATH_TO_STM32CUBEF4}/BSP/STM32F4xx_Nucleo_144/stm32f4xx_nucleo_144.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c

    Src/main.c
    Src/stm32f4xx_it.c
    Src/stm32f4xx_hal_msp.c
    Src/system_stm32f4xx.c
    Src/startup_stm32f429zitx.s
    Src/syscalls.c
    Src/sysmem.c

    ${PATH_TO_LIBTROPIC}/hal/stm32/nucleo_f439zi/libtropic_port_stm32_nucleo_f439zi.c
)

# Include path for directories containing header files
include_directories(PROJ_INCLUDE_DIRS
    ${PATH_TO_STM32CUBEF4}/CMSIS/Core/Include/
    ${PATH_TO_STM32CUBEF4}/CMSIS/Device/ST/STM32F4xx/Include/
    ${PATH_TO_STM32CUBEF4}/BSP/STM32F4xx_Nucleo_144/
    ${PATH_TO_STM32CUBEF4}/STM32F4xx_HAL_Driver/Inc/

    Inc/

    ${PATH_TO_LIBTROPIC}/hal/stm32/nucleo_f439zi/
)

# Add this so HAL knows what target MCU we use
add_definitions(-D${STM32_DEVICE})

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
        add_executable(${exe_name} ${SOURCES})
        target_include_directories(${exe_name} PRIVATE PROJ_INCLUDE_DIRS)
        target_link_libraries(${exe_name} PRIVATE tropic)

        # Apply the strict compilation flags
        # (they are already applied for libtropic from the libtropic's CMakeLists.txt)
        # if(LT_STRICT_COMP_FLAGS)
        #     target_link_libraries(${exe_name} PRIVATE libtropic::strict_comp_flags)
        # endif()

        # Enable example registry using LT_BUILD_EXAMPLES and choose correct example for the binary.
        target_compile_definitions(${exe_name} PRIVATE LT_BUILD_EXAMPLES)
        target_compile_definitions(${exe_name} PRIVATE ${example_macro})

        # Used for testing. Do not use it on when running on standalone Nucleo board
        if (LT_TESTING_RIG)
            target_compile_definitions(${exe_name} PRIVATE LT_TESTING_RIG)
        endif()

        # Use INT pin to check whether TROPIC01 has some response
        # (If not set, by default host will poll every few ms)
        if (LT_USE_INT_PIN)
            set(LT_USE_INT_PIN ON)
            target_compile_definitions(${exe_name} PRIVATE LT_USE_INT_PIN)
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
        add_executable(${exe_name} ${SOURCES})
        target_include_directories(${exe_name} PRIVATE PROJ_INCLUDE_DIRS)
        target_link_libraries(${exe_name} PRIVATE tropic)

        # Apply the strict compilation flags
        # (they are already applied for libtropic from the libtropic's CMakeLists.txt)
        # if(LT_STRICT_COMP_FLAGS)
        #     target_link_libraries(${exe_name} PRIVATE libtropic::strict_comp_flags)
        # endif()

        # Enable test registry using LT_BUILD_TESTS and choose correct test for the binary.
        target_compile_definitions(${exe_name} PRIVATE LT_BUILD_TESTS)
        target_compile_definitions(${exe_name} PRIVATE ${test_macro})

        if(CTEST_PREFIX STREQUAL "")
            set(TEST_NAME_WITH_PREFIX ${test_name})
        else()
            set(TEST_NAME_WITH_PREFIX ${CTEST_PREFIX}_${test_name})
        endif()

        # Used for testing. Do not use it on when running on standalone Nucleo board
        if (LT_TESTING_RIG)
            # Redirect serial output to external UART (instead of using built-in STLink)
            target_compile_definitions(${exe_name} PRIVATE LT_TESTING_RIG)
            
            # Add CTest entry.
            add_test(NAME ${TEST_NAME_WITH_PREFIX}
                    COMMAND python3 -m lt_test_runner ${LT_TEST_RUNNER_FLAGS} stm32_f439zi ${CMAKE_CURRENT_BINARY_DIR}/${exe_name}
            )
            
            # Correctly define Python module path, so we can call lt_test_runner.
            get_filename_component(ABSOLUTE_PATH_TO_LIBTROPIC ${PATH_TO_LIBTROPIC} ABSOLUTE)
            set_tests_properties(${TEST_NAME_WITH_PREFIX} PROPERTIES
                ENVIRONMENT "PYTHONPATH=${ABSOLUTE_PATH_TO_LIBTROPIC}/scripts/test_runner"
            )
        else()
            # Add CTest entry.
            add_test(NAME ${TEST_NAME_WITH_PREFIX}
                    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run_test.sh ${CMAKE_CURRENT_BINARY_DIR}/${exe_name} ${STLINK_UART}
            )
        endif()

        # Use INT pin to check whether TROPIC01 has some response
        # (If not set, by default host will poll every few ms)
        if (LT_USE_INT_PIN)
            set(LT_USE_INT_PIN ON)
            target_compile_definitions(${exe_name} PRIVATE LT_USE_INT_PIN)
        endif()

        # We don't need binary format for now.
        # add_custom_command(TARGET ${exe_name}
        #     POST_BUILD
        #     COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${exe_name}> ${bin_name}
        #     BYPRODUCTS ${bin_name}
        #     COMMENT "Converting ${exe_name} to ${bin_name}"
        # )

    endforeach()
endif()