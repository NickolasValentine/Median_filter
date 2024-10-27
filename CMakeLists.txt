cmake_minimum_required(VERSION 3.10)
project(MedianfilterProject)

# Укажите путь к OpenCV
set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/opencv/build")

# Опция для динамической линковки
set(BUILD_SHARED_LIBS ON)

# Подключите OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Добавьте исполняемый файл и связываем с библиотекой OpenCV
add_executable(Medianfilter main.cpp)
target_link_libraries(Medianfilter ${OpenCV_LIBS})
