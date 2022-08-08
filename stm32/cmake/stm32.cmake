# CMake configuration for compiling stm32 targets

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY NAMES arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP NAMES arm-none-eabi-objdump)
set(CMAKE_SIZE NAMES arm-none-eabi-size)

set(CMAKE_EXECUTABLE_SUFFIX_C .elf)

# Tell CMake to try and make a static library when testing this compiler.
# Otherwise it tries to make an executable but that fails because of "undefined
# reference to _exit"
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

function(stm32_generate_binary_file TARGET)
    add_custom_command(
        TARGET ${TARGET}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET}${CMAKE_EXECUTABLE_SUFFIX_C} ${TARGET}.bin
        BYPRODUCTS ${TARGET}.bin
        COMMENT "Generating binary file ${CMAKE_PROJECT_NAME}.bin"
    )
endfunction()
