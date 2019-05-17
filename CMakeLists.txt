cmake_minimum_required(VERSION 2.8)

# Project VIC
project(VIC CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Executables")
set(CMAKE_SOURCE_DIRECTORY src)
find_package(OpenCV REQUIRED)

# Freenect
set(FreenectExampleModded src/FreenectExampleModded/FreenectExampleModded.h src/FreenectExampleModded/FreenectExampleModded.cpp)
set(FreenectPlaybackWrapper src/FreenectPlaybackWrapper/FreenectPlaybackWrapper.h src/FreenectPlaybackWrapper/FreenectPlaybackWrapper.cpp)
set(Freenect ${FreenectPlaybackWrapper} ${FreenectExampleModded})

# Visual Eyes
set(ImageObject src/VisualEyes/ImageObject/ImageObject.h src/VisualEyes/ImageObject/ImageObject.cpp)
set(VisualEyes ${ImageObject} src/VisualEyes/VisualEyes.h src/VisualEyes/VisualEyes.cpp)

# Source Files
set(SourceFiles ${Freenect} ${VisualEyes})

# Create Executable With Source Files
add_executable(VIC ${SourceFiles} src/VIC.cpp)

# Link OpenCV
target_link_libraries(VIC ${OpenCV_LIBS})
