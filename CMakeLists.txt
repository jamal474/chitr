cmake_minimum_required(VERSION 3.28)
project(Chitr)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CHITR_LOG_LEVEL 1 CACHE STRING "Set Log Level (0=OFF, 1=ERROR, 2=INFO, 3=DEBUG)")

add_compile_definitions(CHITR_LOG_LEVEL=${CHITR_LOG_LEVEL})

# On Linux we consume system wxWidgets (see conanfile.py for the why).
# System wxWidgets requires the "media" component explicitly for wxMediaCtrl
# to link — Conan's cmakedeps exports all built libs so this is only needed here.
if(UNIX AND NOT APPLE)
    find_package(wxWidgets REQUIRED COMPONENTS media net core base)
else()
    find_package(wxWidgets REQUIRED COMPONENTS net core base)
endif()
find_package(iconfontcppheaders REQUIRED)

if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()

set(SOURCES
    src/main.cpp
    src/MainFrame.cpp
    src/media/MediaContext.cpp
    src/media/MediaPanel.cpp
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

# On macOS, build as a proper .app bundle so Finder shows it as an app.
# The icon (MainIcon.icns) is added as a bundle resource so it becomes the
# Finder / Dock icon; runtime assets are still copied by the POST_BUILD steps
# below but now land in Contents/Resources instead of alongside the binary.
if(APPLE)
    set(MACOS_ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/MainIcon.icns")
    add_executable(Chitr MACOSX_BUNDLE ${SOURCES} ${MACOS_ICON})
    set_source_files_properties(${MACOS_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    # Read the version out of the VERSION file so bundle metadata stays in sync.
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION")
        file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" CHITR_VERSION)
        string(STRIP "${CHITR_VERSION}" CHITR_VERSION)
    else()
        set(CHITR_VERSION "0.0.0")
    endif()
    set_target_properties(Chitr PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME           "Chitr"
        MACOSX_BUNDLE_GUI_IDENTIFIER        "com.chitr.chitr"
        MACOSX_BUNDLE_BUNDLE_VERSION        "${CHITR_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING  "${CHITR_VERSION}"
        MACOSX_BUNDLE_ICON_FILE             "MainIcon.icns"
    )
else()
    add_executable(Chitr ${SOURCES})
endif()

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

# Runtime asset destination:
#   * macOS bundle -> Chitr.app/Contents/Resources
#   * Linux/Windows -> sibling folder of the executable
if(APPLE)
    set(CHITR_RESOURCE_DIR "$<TARGET_FILE_DIR:Chitr>/../Resources")
else()
    set(CHITR_RESOURCE_DIR "$<TARGET_FILE_DIR:Chitr>")
endif()

set(FONT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets/font_ttf/material_v1.ttf")
add_custom_command(TARGET Chitr POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CHITR_RESOURCE_DIR}/Fonts"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FONT_SOURCE}"
        "${CHITR_RESOURCE_DIR}/Fonts/material_v1.ttf"
    COMMENT "Copying Fonts"
)

set(ICON_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets/MainIcon.png")
add_custom_command(TARGET Chitr POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CHITR_RESOURCE_DIR}/Assets"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ICON_SOURCE}"
        "${CHITR_RESOURCE_DIR}/Assets/MainIcon.png"
    COMMENT "Copying Icon"
)
