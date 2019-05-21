cmake_minimum_required(VERSION 2.8)

# Project VIC
project(VIC CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Executables")
set(CMAKE_SOURCE_DIRECTORY src)
find_package(OpenCV REQUIRED)

# Freenect
set(FenVIC src/FenVIC/FenVIC.h src/FenVIC/FenVIC.cpp)
# Visual Eyes
set(ImageObject src/VisuII/ImageObject/ImageObject.h src/VisuII/ImageObject/ImageObject.cpp)
set(VisuII ${ImageObject} src/VisuII/VisuII.h src/VisuII/VisuII.cpp)
# Source Files
set(SourceFiles ${FenVIC} ${VisuII})

# Create Executable With Source Files
add_executable(VIC ${SourceFiles} src/VIC.cpp)

# Link OpenCV
target_link_libraries(VIC ${OpenCV_LIBS})
