# Simple Network Game

This project is a simple multiplayer game implemented using WebSocket for real-time communication between a C++ server and web-based clients.

## Prerequisites

- C++ compiler with C++11 support
- CMake (version 3.10 or higher)
- websocketpp library
- Boost library
- A modern web browser

## Building and Running

### Server

1. Navigate to the server directory:
   ```
   cd server
   ```


2. Run CMake and build the project:
   ```
   cmake .
   make
   ```

4. Run the server:
   ```
   ./server
   ```

### Client

1. Open the `client/index.html` file in a web browser.

2. Use the arrow keys to move your player.

