# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PROGRAM_NAME turtle)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(turtle C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

message("Running makefsdata python script")
execute_process(COMMAND
    py makefsdata.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

# Add executable. Default name is the project name, version 0.1

add_executable(${PROGRAM_NAME}
    turtle.cpp
)

pico_set_program_name(${PROGRAM_NAME} "turtle")
pico_set_program_version(${PROGRAM_NAME} "0.1")

pico_generate_pio_header(${PROGRAM_NAME} ${CMAKE_CURRENT_LIST_DIR}/quadrature_encoder.pio)

target_sources(${PROGRAM_NAME} PRIVATE turtle.cpp)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${PROGRAM_NAME} FALSE)
pico_enable_stdio_usb(${PROGRAM_NAME} TRUE)

# Add the standard library to the build
target_link_libraries(${PROGRAM_NAME}
        pico_stdlib
        )

# Add the standard include files to the build
target_include_directories(${PROGRAM_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(${PROGRAM_NAME} 
        hardware_pio
        hardware_pwm
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_http  
    )

pico_add_extra_outputs(${PROGRAM_NAME})

