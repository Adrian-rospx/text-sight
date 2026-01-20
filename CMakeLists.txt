cmake_minimum_required(VERSION 4.0)

project(text-sight 
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "TUI visualizer application"
)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV CONFIG REQUIRED)
find_package(ftxui CONFIG REQUIRED)

add_executable(textsight
    src/main.cpp
    src/app/app_controller.cpp
    src/services/camera.cpp
    src/ui/controller.cpp
    src/ui/image_canvas.cpp
    src/ui/view.cpp
)

target_include_directories(textsight 
    PRIVATE src
)

target_link_libraries(textsight PRIVATE
    opencv_core
    opencv_imgproc
    opencv_highgui
    ftxui::screen
    ftxui::dom
    ftxui::component
)