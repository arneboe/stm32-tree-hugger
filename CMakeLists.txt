cmake_minimum_required(VERSION 3.7)
project(stm32-tree-hugger)

enable_language(ASM)

add_definitions(-DSTM32F103xB)
add_definitions(-DARM_MATH_CM3)
add_definitions(-DUSE_HAL_DRIVER)

set(ASM_SOURCES startup_stm32f103xb.s)
file(GLOB_RECURSE HAL_SOURCES Drivers/STM32F1xx_HAL_Driver/Src/*.c)
file(GLOB_RECURSE USB_SOURCES Middlewares/ST/STM32_USB_Device_Library/Core/Src/*.c)
file(GLOB_RECURSE USB_CDC_SOURCES Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/*.c)

set(DSP /home/arne/STM32Cube/Repository/STM32Cube_FW_F1_V1.7.0/Drivers/CMSIS/DSP_Lib/Source/)

file(GLOB_RECURSE CXX_FILES User_Src/*.cpp)
file(GLOB_RECURSE USR_C_FILES User_Src/*.c)
file(GLOB_RECURSE C_FILES Src/*.c)

set(SOURCES ${HAL_SOURCES} ${USB_SOURCES} ${CXX_FILES} ${C_FILES} ${ASM_SOURCES} ${USB_CDC_SOURCES} ${ROS_SOURCES} ${DXL_SOURCES} ${USR_C_FILES} User_Src/syscalls.c
        ${DSP}/TransformFunctions/arm_cfft_radix4_f32.c
        ${DSP}/ComplexMathFunctions/arm_cmplx_mag_f32.c
        ${DSP}/TransformFunctions/arm_bitreversal.c
        ${DSP}/TransformFunctions/arm_cfft_radix4_init_f32.c
        ${DSP}/CommonTables/arm_common_tables.c)




include_directories(Drivers/CMSIS/Device/ST/STM32F1xx/Include)
include_directories(Drivers/CMSIS/Include)
include_directories(Drivers/STM32F1xx_HAL_Driver/Inc)
include_directories(Middlewares/ST/STM32_USB_Device_Library/Core/Inc/)
include_directories(Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/)
include_directories(Inc)
include_directories(User_Src)


add_executable(${PROJECT_NAME}.elf ${SOURCES})
