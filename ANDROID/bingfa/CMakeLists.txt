cmake_minimum_required(VERSION 3.16)
project(bingfa C)

set(CMAKE_C_STANDARD 99)

include_directories(~/workspace/android-2.3.7_r1/kernel/goldfish2.6.29/include)
include_directories(~/workspace/android-2.3.7_r1/kernel/goldfish2.6.29/arch/arm/mach-s3c6400/include)
include_directories(/home/jrx/workspace/android-2.3.7_r1/kernel/goldfish2.6.29/arch/arm/plat-s3c64xx/include)
include_directories(/home/jrx/workspace/android-2.3.7_r1/kernel/goldfish2.6.29/arch/parisc/include)
include_directories(/home/jrx/workspace/android-2.3.7_r1/hardware/libhardware/include)
include_directories(/home/jrx/workspace/android-2.3.7_r1/system/core/include)
add_executable(bingfa main.c yuanzicaozuo.c zixuansuo.c semaphore.c)