cmake_minimum_required(VERSION 3.16)
project(MyGedit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# Resource file that embeds icons/image.png
qt6_add_resources(RESOURCES resources.qrc)

add_executable(mygedit
    src/main.cpp
    ${RESOURCES}
)

target_link_libraries(mygedit Qt6::Core Qt6::Widgets)

# Installation: binary goes to bin/
install(TARGETS mygedit RUNTIME DESTINATION bin)
