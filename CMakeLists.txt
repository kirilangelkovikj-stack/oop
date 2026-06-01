cmake_minimum_required(VERSION 3.20)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(raygui_master CXX)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

set(SOURCE_DIR "raygui-master/sources")
include_directories(${SOURCE_DIR})

add_executable(raygui_master
        ${SOURCE_DIR}/main.cpp
)

target_link_libraries(raygui_master PRIVATE
        raylib
        opengl32
        gdi32
        winmm
        user32
        kernel32
        shell32
        comdlg32
        advapi32
        setupapi
        imm32
        version
)