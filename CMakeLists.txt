# This file defines to the code editor how to build the project. CMake is not super important for you right now, so
# we will not go into detail

# Tell CMake the minimum version needed to work
cmake_minimum_required(VERSION 4.2)

# Declare an app name
project(notepad_app)

# Set what language version we are using (in this case, C++23)
set(CMAKE_CXX_STANDARD 23)

# Make sure to clone the git submodules containing the ImGui and GLFW libraries
execute_process(COMMAND git submodule update --init)

# Add libs/glfw as a dependency directory. This directory also contains a CMakeLists.txt file, so CMake will load the
# targets in there and make them available to us in our projects CMakeLists.txt
add_subdirectory(libs/glfw)

# ImGui does not ship with a CMakeLists.txt, so we have to construct the library ourselves
add_library(imgui OBJECT
        libs/imgui/imgui.cpp
        libs/imgui/imgui_demo.cpp
        libs/imgui/imgui_draw.cpp
        libs/imgui/imgui_tables.cpp
        libs/imgui/imgui_widgets.cpp
        libs/imgui/backends/imgui_impl_opengl3.cpp
        libs/imgui/backends/imgui_impl_glfw.cpp
)

# Set up the libraries and include directories for ImGui
target_include_directories(imgui PUBLIC libs/imgui libs/imgui/backends)
target_compile_definitions(imgui PUBLIC -DIMGUI_DEFINE_MATH_OPERATORS)
target_link_libraries(imgui PUBLIC glfw opengl32)

# Create our app's executable. Also indicate we have one source file
add_executable(notepad_app src/main.cpp)

# Provide the information on what libraries we depend on (ImGui, and by extension GLFW and openGL)
target_include_directories(notepad_app PRIVATE include)
target_link_libraries(notepad_app PRIVATE imgui)

# Tell the compiler to do some other special things to make the program optimized for Windows
target_link_options(notepad_app PRIVATE -mwindows -static)
