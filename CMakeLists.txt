cmake_minimum_required(VERSION 3.10)

project(Simplicity LANGUAGES CXX ASM VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

set(CXXFLAGS -Wall -Wextra -Wpedantic -g -pipe)

set(LINKER_SCRIPT ../src/linker.ld)

set(INTERNALCXXFLAGS -ffreestanding -fno-stack-protector -fno-pic -fpie
                     -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2
                     -mno-red-zone)
set(INTERNALLDFLAGS -fno-pic -fpie -Wl,-static,-pie,--no-dynamic-linker,-ztext
                    -static-pie -nostdlib -T${LINKER_SCRIPT}
                    -z max-page-size=0x1000)

add_executable(simplicity.elf src/Kernel.cpp src/GDT.cpp)
target_compile_options(simplicity.elf PUBLIC ${CXXFLAGS} ${INTERNALCXXFLAGS})
target_link_options(simplicity.elf PUBLIC ${INTERNALLDFLAGS})

