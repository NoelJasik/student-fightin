cmake_minimum_required(VERSION 3.15)
project(uniProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Teraz SDL2 pobiera się z automatu
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.5
)

FetchContent_MakeAvailable(SDL2)

add_executable(uniProject main.cpp
        gameObject.cpp
        gameObject.h
        klasy/button.cpp
        klasy/button.h) # obiera buttona

target_link_libraries(uniProject PRIVATE SDL2::SDL2main SDL2::SDL2-static) #Jak coś tego nie dotykać bo sie wszystko rozwali z jakiegoś powodu xD

target_include_directories(uniProject PRIVATE
        ${sdl2_SOURCE_DIR}/include
)
