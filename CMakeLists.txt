# Do not modify this file unless absolutely necessary.
# All project-specific settings can be found in the file:
# project_config.cmake

cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(DEFINED ENV{NGAGESDK})
  set(NGAGESDK $ENV{NGAGESDK})
  set(CMAKE_TOOLCHAIN_FILE ${NGAGESDK}/cmake/ngage-toolchain.cmake)
else()
  message(FATAL_ERROR "The environment variable NGAGESDK needs to be defined.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/mmc/system/updates)

project(WSODWarrior C CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/project_config.cmake)

set(UID1 0x1000007a) # KExecutableImageUidValue, e32uid.h
set(UID2 0x100039ce) # KAppUidValue16, apadef.h

set(GCC_COMN_DEFS -D__SYMBIAN32__ -D__GCC32__ -D__EPOC32__ -D__MARM__ -D__MARM_ARMI__)
set(GCC_MODE_DEFS -DNDEBUG -D_UNICODE)
set(GCC_DEFS      ${GCC_COMN_DEFS} ${GCC_MODE_DEFS})

set(wsodwarrior_libs
  ${EPOC_PLATFORM}/gcc/lib/gcc-lib/arm-epoc-pe/2.9-psion-98r2/libgcc.a
  ${EPOC_LIB}/egcc.lib
  ${EPOC_LIB}/euser.lib
  ${EPOC_LIB}/estlib.lib
  ${EPOC_LIB}/ws32.lib
  ${EPOC_LIB}/hal.lib
  ${EPOC_LIB}/efsrv.lib)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(wsodwarrior STATIC ${project_sources})

build_exe(wsodwarrior exe ${UID1} ${UID2} ${UID3} "${wsodwarrior_libs}")

copy_file(
    wsodwarrior.exe
    ${CMAKE_CURRENT_BINARY_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/mmc/system/updates"
    "wsodwarrior.exe")

add_dependencies(
    wsodwarrior.exe
    wsodwarrior)

target_compile_definitions(
    wsodwarrior
    PUBLIC
    __EXE__
    FUNCTION_NAME=__FUNCTION__
    ${GCC_DEFS}
    UID1=${UID1}
    UID2=${UID2}
    UID3=${UID3})

target_compile_options(
    wsodwarrior
    PUBLIC
    -Wall
    -O3)

target_include_directories(
    wsodwarrior
    PUBLIC
    ${SRC_DIR})
