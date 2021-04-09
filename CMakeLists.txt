cmake_minimum_required(VERSION 3.14)
project(dictionary CXX)

add_compile_options("-std=c++17")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "/usr/lib/qt5/bin")
set(CMAKE_AUTOMOC ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Threads REQUIRED)

add_library(dictionary
        src/dictionary.cpp
        src/dictionary.h)

add_executable(main_gui
        src/main.cpp
        src/DictionaryWidget.cpp
        src/DictionaryWidget.h)

target_link_libraries(main_gui Qt5::Widgets Qt5::Core Qt5::Gui dictionary)

target_link_libraries(dictionary Threads::Threads)

set_target_properties(main_gui dictionary
        PROPERTIES
        LINKER_LANGUAGE CXX
        CMAKE_CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/words.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
