cmake_minimum_required(VERSION 3.24) # Nowsza wersja, lepiej powinno śmigać




project(uniproject VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. Konfiguracja statycznego buildu ---
# Trzeba to zrobić przed FetchContent_Declare, zmusza to SDL żeby skompilował się statycznie na pliki, dzięki temu nie trzeba DLL'sów jak się skompiluje

option(SDL_SHARED "Build shared SDL" OFF)
option(SDL_STATIC "Build static SDL" ON)
option(SDL2_DISABLE_INSTALL "Disable installing SDL2" ON)

# Zmuszamy SDL'a do skompilowania statycznie
option(SDL2_IMAGE_BUILD_SHARED_LIBS "Build shared SDL2_image" OFF)
# Wyłączamy testy żeby szybciej szło
option(SDL2_IMAGE_BUILD_TESTS "Build SDL2_image tests" OFF)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libs" FORCE)
set(SDL2_SHARED OFF CACHE BOOL "Link SDL2 shared" FORCE)
set(SDL2_IMAGE_SHARED OFF CACHE BOOL "Link SDL2_image shared" FORCE)
set(SDL2_TTF_SHARED OFF CACHE BOOL "Link SDL2_ttf shared" FORCE)

include(FetchContent)

# --- 2. Pobieranie dependencies (ja zawsze talk pół polish pół english ---
# SDL2 (Core)
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-2.30.5
)
FetchContent_MakeAvailable(SDL2)
# SDL2_image (Extension)
FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG        release-2.8.8
)

FetchContent_Declare( # to coś pobiera z gita ttf czyli do renderowania napisów (dawid)
        SDL2_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.24.0 #MUSI BYć WERSJA Z 2 NA POCZĄTKU
)
set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2_ttf)
#
# START ADDITION
set(SDL2IMAGE_INSTALL OFF)
set(BUILD_SHARED_LIBS FALSE)
# END ADDITION

FetchContent_MakeAvailable(SDL2_image)

# --- 3. dodajemy do exe (JAK STWORZYCIE NOWE KLASY/HEADERY TO TU TRZEBA DODAĆ) ---
add_executable(uniProject
        main.cpp
        klasy/gameObject.cpp
        headers/gameObject.h
        klasy/button.cpp
        headers/button.h
)

# --- 4. Dodajemy ścieżki do naszych bibliotek (mam nadzieje że windows to zrozumie) ---
# Dzięki temu można pisać #include "button.h"
# zamiast "../headeary/button.h"
# Ale nwm czy to na windowsie będzie działać to narazie nie zmieniam
target_include_directories(uniProject PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

# --- 5. Podłączamy biblioteki ---
target_link_libraries(uniProject PRIVATE
        # używamy statycznych blibliotek
        SDL2_image::SDL2_image-static
        SDL2::SDL2-static
        SDL2_ttf::SDL2_ttf-static
        # Z dedykacją dla winowsa :D
        # Na systemach opartych na unixie nie powinno nic zmieniać
        SDL2::SDL2main
)

# --- 6. Kompilacja assetów ---
# Dodajemy folder assets żeby był zawarty w pliku exe, audio też by tu trzeba było dodać
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    add_custom_command(TARGET uniProject POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:uniProject>/assets"
    )
endif()