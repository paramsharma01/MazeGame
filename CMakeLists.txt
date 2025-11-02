cmake_minimum_required(VERSION 3.10)
project(MazeGame)

# Use modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML installed via Homebrew
# (Homebrew installs SFML 2.6.x to /opt/homebrew/Cellar/sfml@2.6.2_1)
set(SFML_DIR "/opt/homebrew/Cellar/sfml@2.6.2_1/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)

# Include directories
include_directories(include)

# Add your source files
add_executable(MazeGame
    src/main.cpp
    src/Maze.cpp
    src/Player.cpp
    src/Timer.cpp

)

# Link SFML libraries
target_link_libraries(MazeGame
    sfml-system
    sfml-window
    sfml-graphics
)

# Optional: macOS specific frameworks (needed for windowing/audio)
if(APPLE)
    target_link_libraries(MazeGame "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation")
endif()
