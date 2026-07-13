cmake_minimum_required(VERSION 4.2)
project(notepad_app)

set(CMAKE_CXX_STANDARD 23)

execute_process(COMMAND git submodule update --init)

add_subdirectory(libs/glfw)

add_library(imgui
        libs/imgui/imgui.cpp
        libs/imgui/imgui_demo.cpp
        libs/imgui/imgui_draw.cpp
        libs/imgui/imgui_tables.cpp
        libs/imgui/imgui_widgets.cpp
        libs/imgui/backends/imgui_impl_opengl3.cpp
        libs/imgui/backends/imgui_impl_glfw.cpp
)

target_include_directories(imgui PUBLIC libs/imgui libs/imgui/backends)
target_compile_definitions(imgui PUBLIC -DIMGUI_DEFINE_MATH_OPERATORS)
target_link_libraries(imgui PUBLIC glfw opengl32)

add_executable(notepad_app src/main.cpp)
target_include_directories(notepad_app PRIVATE include)
target_link_libraries(notepad_app PRIVATE imgui)
target_link_options(notepad_app PRIVATE -mwindows)
