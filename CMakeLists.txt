cmake_minimum_required(VERSION 3.16)
project(TopoTactics LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Find SFML
# find_package(SFML 2.5 COMPONENTS system window graphics network REQUIRED)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Find OpenGL package
find_package(OpenGL REQUIRED)

set(INCLUDE_DIR
${CMAKE_SOURCE_DIR}/headers
${CMAKE_SOURCE_DIR}/headers/Cards
${CMAKE_SOURCE_DIR}/headers/Board
${CMAKE_SOURCE_DIR}/headers/UI
${CMAKE_SOURCE_DIR}/imgui
)

# include_directories("${CMAKE_SOURCE_DIR}/headers" "${CMAKE_SOURCE_DIR}/headers/Cards"  "${CMAKE_SOURCE_DIR}/headers/Board" "${CMAKE_SOURCE_DIR}/headers/UI" "${CMAKE_SOURCE_DIR}/imgui")
file(GLOB_RECURSE SERVERSRC
"${CMAKE_SOURCE_DIR}/src/Board/*.cpp"
"${CMAKE_SOURCE_DIR}/src/Cards/*.cpp"
"${CMAKE_SOURCE_DIR}/src/UI/*.cpp"
"${CMAKE_SOURCE_DIR}/src/Server/*.cpp"
)

file(GLOB_RECURSE CLIENTSRC
"${CMAKE_SOURCE_DIR}/src/Board/*.cpp"
"${CMAKE_SOURCE_DIR}/src/Cards/*.cpp"
"${CMAKE_SOURCE_DIR}/src/UI/*.cpp"
"${CMAKE_SOURCE_DIR}/src/TextureManager.cpp"
"${CMAKE_SOURCE_DIR}/src/Client/*.cpp")

# text file directory
file(COPY "${CMAKE_SOURCE_DIR}/src/Board/files" DESTINATION "${CMAKE_BINARY_DIR}")
# assets directory
file(COPY "${CMAKE_SOURCE_DIR}/src/Board/assets" DESTINATION "${CMAKE_BINARY_DIR}")

# ImGui source files
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/imgui")

# Add ImGui files
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui-SFML.cpp
    # Add other necessary files like imgui_impl_sfml.cpp, imgui-SFML.cpp, etc. if you're using them.
)

add_executable(TTServer ${SERVERSRC})
target_link_libraries(TTServer sfml-window sfml-graphics sfml-network)
target_include_directories(TTServer PRIVATE ${INCLUDE_DIR})
target_compile_features(TTServer PRIVATE cxx_std_17)

add_executable(TTClient ${CLIENTSRC})
target_link_libraries(TTClient sfml-window sfml-graphics sfml-network)
target_include_directories(TTClient PRIVATE ${INCLUDE_DIR})
target_compile_features(TTClient PRIVATE cxx_std_17)

install(TARGETS TTServer)
install(TARGETS TTClient)
