cmake_minimum_required(VERSION 3.10)
project(WebSocketServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)

# Add nlohmann/json
include(FetchContent)
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2)
FetchContent_MakeAvailable(json)

# Add WebSocket++ include directory
set(WEBSOCKETPP_INCLUDE_DIR "/usr/include" CACHE PATH "WebSocket++ include directory")
include_directories(${WEBSOCKETPP_INCLUDE_DIR})

# Add all source files
set(SOURCES
    main.cpp
    WebSocketServer.cpp
    GameState.cpp
)

# Add all header files
set(HEADERS
    WebSocketServer.hpp
    GameState.hpp
    Player.hpp
    GameAction.hpp
)

# Add the executable
add_executable(server ${SOURCES} ${HEADERS})

# Link against required libraries
target_link_libraries(server PRIVATE Threads::Threads nlohmann_json::nlohmann_json)

# Add compiler warnings
if(MSVC)
    target_compile_options(server PRIVATE /W4)
else()
    target_compile_options(server PRIVATE -Wall -Wextra -pedantic)
endif()

# Print configuration summary
message(STATUS "WebSocket++ include dir: ${WEBSOCKETPP_INCLUDE_DIR}")