cmake_minimum_required(VERSION 3.10)

# Nazwa projektu
project(KolejkaPriorytetowa)

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#folder z nagłówkami
include_directories(include)

# Automatyczne znalezienie wszystkich plików .cpp w folderze src
file(GLOB SOURCES "src/*.cpp")

# Stworzenie pliku wykonywalnego
add_executable(PriorityQueue ${SOURCES})