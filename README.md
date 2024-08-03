# Simple Network Game

This project is a simple multiplayer game implemented using WebSocket for real-time communication between a C++ server and web-based clients.

## Features

- Real-time multiplayer gameplay
- C++ WebSocket server using websocketpp
- Web-based client using HTML5 and JavaScript
- Player movement using arrow keys

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

2. Create a build directory and navigate into it:
   ```
   mkdir build && cd build
   ```

3. Run CMake and build the project:
   ```
   cmake ..
   make
   ```

4. Run the server:
   ```
   ./server
   ```

### Client

1. Open the `client/index.html` file in a web browser.

2. Use the arrow keys to move your player.

## Contributing

Feel free to fork the project and submit pull requests with any improvements or bug fixes.

## License

[Specify your license here, e.g., MIT, GPL, etc.]

## Author

[Your Name]
