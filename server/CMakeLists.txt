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

# Add Google Test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# Add WebSocket++ include directory
set(WEBSOCKETPP_INCLUDE_DIR "/usr/include" CACHE PATH "WebSocket++ include directory")
include_directories(${WEBSOCKETPP_INCLUDE_DIR})

# Add all source files
set(SOURCES
    WebSocketServer.cpp
    GameState.cpp
    GameBoard.cpp
    Tile.cpp
    TileFactory.cpp
)

# Add all header files
set(HEADERS
    WebSocketServer.hpp
    GameState.hpp
    GameBoard.hpp
    Player.hpp
    GameAction.hpp
    Tile.hpp
    TileFactory.hpp
)

# Add the main executable
add_executable(server main.cpp ${SOURCES} ${HEADERS})

# Add the test executable
add_executable(server_tests GameStateTests.cpp ${SOURCES} ${HEADERS})

# Link against required libraries for main executable
target_link_libraries(server PRIVATE Threads::Threads nlohmann_json::nlohmann_json)

# Link against required libraries for test executable
target_link_libraries(server_tests PRIVATE Threads::Threads nlohmann_json::nlohmann_json gtest gtest_main)

# Add compiler warnings
if(MSVC)
    target_compile_options(server PRIVATE /W4)
    target_compile_options(server_tests PRIVATE /W4)
else()
    target_compile_options(server PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(server_tests PRIVATE -Wall -Wextra -pedantic)
endif()

# Print configuration summary
message(STATUS "WebSocket++ include dir: ${WEBSOCKETPP_INCLUDE_DIR}")

# Add custom target to run tests
add_custom_target(run_tests
    COMMAND server_tests
    DEPENDS server_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Enable testing
enable_testing()
add_test(NAME server_tests COMMAND server_tests)
