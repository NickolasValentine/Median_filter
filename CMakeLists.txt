cmake_minimum_required(VERSION 3.10)
project(MedianfilterProject)

# ������� ���� � OpenCV
# ����� ��� ������������ ��������
# ������� ���� � ����� include � ����������� OpenCV
set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/opencv/build/x64/vc16/lib")
# ���������� OpenCV
find_package(OpenCV REQUIRED)

# ������� ���� � include � lib
include_directories(${CMAKE_SOURCE_DIR}/opencv/include)
link_directories(${CMAKE_SOURCE_DIR}/opencv/x64/vc16/lib)

# ������� �������� ����� �������
# �������� � OpenCV
# �������� ����������� ���� � ��������� � ����������� OpenCV
add_executable(Medianfilter main.cpp)
target_link_libraries(Medianfilter ${OpenCV_LIBS})
