cmake_minimum_required(VERSION 3.20)
project(FingerGame LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SFML "Build the SFML UI app" ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

set(COMMON_SOURCES
    GameCode.cpp
    EnemyPlayer.cpp
    UserPlayer.cpp
    master.cpp
)

if(BUILD_SFML)
    add_executable(app ${COMMON_SOURCES} main_sfml.cpp)
else()
    add_executable(app ${COMMON_SOURCES} main.cpp)
endif()

target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(app PRIVATE SFML::Graphics SFML::Window SFML::System)

# Copy assets after build so app.exe can load sprites/fonts at runtime
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:app>/assets
)
