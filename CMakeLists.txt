cmake_minimum_required(VERSION 3.20)
project(FingerGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Choose which "main" to build ---
# ON  => build SFML windowed app (main_sfml.cpp)
# OFF => build console app (main.cpp)
option(BUILD_SFML "Build the SFML UI app" ON)

# Find SFML 3 (installed via vcpkg)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

# Common sources used by both apps — add the files you actually have
set(COMMON_SOURCES
    GameCode.cpp
    EnemyPlayer.cpp
    UserPlayer.cpp
    master.cpp
    # add any other actual .cpp files you have (do NOT put cpu.cpp unless it exists)
)

if(BUILD_SFML)
    add_executable(app
        ${COMMON_SOURCES}
        main_sfml.cpp   # SFML entry point
    )
else()
    add_executable(app
        ${COMMON_SOURCES}
        main.cpp        # Console entry point
    )
endif()

target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(app PRIVATE SFML::Graphics SFML::Window SFML::System)
