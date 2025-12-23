cmake_minimum_required(VERSION 3.15)
project(PhotoViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CHITR_LOG_LEVEL 1 CACHE STRING "Set Log Level (0=OFF, 1=ERROR, 2=INFO, 3=DEBUG)")

add_compile_definitions(CHITR_LOG_LEVEL=${CHITR_LOG_LEVEL})

find_package(wxWidgets REQUIRED COMPONENTS net core base)
find_package(iconfontcppheaders REQUIRED)

if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()

set(SOURCES
    src/main.cpp
    src/RootFrame.cpp
    src/ImagePlayback.cpp
    src/VideoPlayback.cpp
    src/Resource.cpp
    src/utils/ChitrLogger.cpp
)

add_executable(PhotoViewer ${SOURCES})

if(MSVC)
    set_target_properties(PhotoViewer PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()

target_include_directories(PhotoViewer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(PhotoViewer PRIVATE 
    ${wxWidgets_LIBRARIES} 
    iconfontcppheaders::iconfontcppheaders
)

target_compile_options(PhotoViewer PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-w>
    $<$<CXX_COMPILER_ID:MSVC>:/W0>
)

set(FONT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets/font_ttf/material_v1.ttf")
add_custom_command(TARGET PhotoViewer POST_BUILD
   
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:PhotoViewer>/Fonts"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FONT_SOURCE}"
    "$<TARGET_FILE_DIR:PhotoViewer>/Fonts/material_v1.ttf"
    
    COMMENT "Copying Fonts to output directory"
)