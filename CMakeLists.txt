cmake_minimum_required(VERSION 3.10)
project(MedianfilterProject)

# Укажите путь к OpenCV
# Опция для динамической линковки
# Укажите путь к папке include и библиотекам OpenCV
set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/opencv/build/x64/vc16/lib")
# Подключите OpenCV
find_package(OpenCV REQUIRED)

# Укажите путь к include и lib
include_directories(${CMAKE_SOURCE_DIR}/opencv/include)
link_directories(${CMAKE_SOURCE_DIR}/opencv/x64/vc16/lib)

# Укажите исходные файлы проекта
# Линковка с OpenCV
# Добавьте исполняемый файл и связываем с библиотекой OpenCV
add_executable(Medianfilter main.cpp)
target_link_libraries(Medianfilter ${OpenCV_LIBS})
