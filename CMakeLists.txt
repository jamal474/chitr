cmake_minimum_required(VERSION 3.0)
project(PhotoViewer)

set(CMAKE_CXX_STANDARD 17)          
set(CMAKE_CXX_STANDARD_REQUIRED ON)  
set(CMAKE_CXX_EXTENSIONS OFF)      

find_package(wxWidgets REQUIRED COMPONENTS net core base)
find_package(iconfontcppheaders REQUIRED)

if(wxWidgets_USE_FILE) 
    include(${wxWidgets_USE_FILE})
endif()

include_directories(${CMAKE_SOURCE_DIR}/headers)

add_executable(PhotoViewer src/main.cpp
                      src/RootFrame.cpp
                      src/ImagePlayback.cpp 
                      src/VideoPlayback.cpp 
                      src/Resource.cpp)

target_link_libraries(PhotoViewer ${wxWidgets_LIBRARIES} iconfontcppheaders::iconfontcppheaders)
target_compile_options(PhotoViewer PRIVATE 
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-w> 
    $<$<CXX_COMPILER_ID:MSVC>:/W0>         
)

set(FONT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets/font_ttf/material_v1.ttf")
set(FONT_DEST "${CMAKE_BINARY_DIR}/Fonts/material_v1.ttf")

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Fonts")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${FONT_SOURCE}
    ${FONT_DEST}
)
