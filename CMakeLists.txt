cmake_minimum_required(VERSION 3.30)
project(audio-crypt)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML REQUIRED COMPONENTS audio)
find_package(FFTW3 REQUIRED)

include_directories(include lib)

add_executable(audio_crypt src/main.cpp
        lib/CLI11/CLI11.hpp
        src/freq_decode.cpp
        include/freq_decode.h
        src/freq_encode.cpp
        include/freq_encode.h)

target_link_libraries(audio_crypt sfml-audio fftw3)
