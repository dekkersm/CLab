cmake_minimum_required(VERSION 3.21)
project(maman14 C)

set(CMAKE_C_STANDARD 90)

add_executable(maman14 preprocessor.c preprocessor.h globals.h utils.c firstRun.h firstRun.c dataStructs.c dataStructs.h secondRun.c secondRun.h lineParsing.c lineParsing.h Assembler.c)
