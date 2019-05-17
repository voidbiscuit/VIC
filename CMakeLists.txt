cmake_minimum_required(VERSION 2.8)

PROJECT(VIExamples)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Executables")


find_package(OpenCV REQUIRED)

# Modify the following line to add new files to your project 
add_executable(VisualEyes FreenectExampleModded/FreenectExampleModded.h FreenectExampleModded/FreenectExampleModded.cpp FreenectPlaybackWrapper/FreenectPlaybackWrapper.h FreenectPlaybackWrapper/FreenectPlaybackWrapper.cpp VisualEyes/ImageObject/ImageObject.h VisualEyes/ImageObject/ImageObject.cpp VisualEyes/VisualEyes.h VisualEyes/VisualEyes.cpp Main.cpp)
 
target_link_libraries(VisualEyes ${OpenCV_LIBS})
