cmake_minimum_required(VERSION 3.28)
project(Chitr)

set(CMAKE_CXX_STANDARD 20)
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
    src/MainFrame.cpp
    src/media/MediaContext.cpp
    src/media/ImagePanel.cpp
    src/media/ImageContext.cpp
    src/media/VideoPanel.cpp
    src/media/VideoContext.cpp
    src/utils/ChitrLogger.cpp
    src/utils/Resource.cpp
    src/utils/CTime.cpp
    src/utils/CFile.cpp
    src/component/RoundedText.cpp
)

add_executable(Chitr ${SOURCES})

if(MSVC)
    set_target_properties(Chitr PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()

target_include_directories(Chitr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Chitr PRIVATE 
    ${wxWidgets_LIBRARIES} 
    iconfontcppheaders::iconfontcppheaders
)

target_compile_options(Chitr PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-w>
    $<$<CXX_COMPILER_ID:MSVC>:/W0>
)

set(FONT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets/font_ttf/material_v1.ttf")
add_custom_command(TARGET Chitr POST_BUILD
   
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Chitr>/Fonts"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FONT_SOURCE}"
    "$<TARGET_FILE_DIR:Chitr>/Fonts/material_v1.ttf"
    
    COMMENT "Copying Fonts to output directory"
)

set(ICON_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets/MainIcon.png")
add_custom_command(TARGET Chitr POST_BUILD
   
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Chitr>/Assets"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${ICON_SOURCE}"
    "$<TARGET_FILE_DIR:Chitr>/Assets/MainIcon.png"
    
    COMMENT "Copying Icon to output directory"
)
