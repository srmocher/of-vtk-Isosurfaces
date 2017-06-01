project(ofVtkIsoSurfaces)
set(APP ofVtkIsoSurfaces)

cmake_minimum_required(VERSION 3.3)

set(OF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../of/" CACHE PATH "The root directory of ofnode/of project.")
include(${OF_ROOT}/openFrameworks.cmake)


find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


ofxaddon(ofxGui)


set(SOURCES
    src/main.cpp
    src/ofApp.cpp
    src/marchingcubes.cpp
        )

add_executable(
    ${APP}
    MACOSX_BUNDLE
    ${SOURCES}
    ${OFXADDONS_SOURCES}
)

target_link_libraries(
    ${APP}
    ${OPENFRAMEWORKS_LIBRARIES}
    ${VTK_LIBRARIES}
)
