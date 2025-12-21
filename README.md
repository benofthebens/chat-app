# chat-app

A simple client-server chat application built with Winsock2 & Win32 API. This project demonstrates low-level network programming and custom GUI framework development, helping to understand how libraries like wxWidgets and Qt work under the hood.

## Features

- Client-server architecture with TCP sockets
- Custom event-driven GUI framework ("Engine")
- Layer-based application architecture
- Template-based protocol serialization
- Multi-threaded server supporting multiple concurrent clients
- Custom widget system (Panel, Button, TextInput)

## Table of Contents
- [Requirements](#requirements)
- [Dependencies](#dependencies)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Building](#building)
- [Usage](#usage)
- [Architecture](#architecture)
- [Testing](#testing)
- [Known Limitations](#known-limitations)
- [License](#license)

## Requirements

- C++17 compiler (MSVC recommended)
- Windows OS
- CMake 3.10+

## Dependencies

- [Google Test](https://github.com/google/googletest.git) (included as submodule)
- Winsock2 (ws2_32.lib)
- Win32 API

## Project Structure
```
chat-app/
├── app/                    # Main application
│   ├── src/               # Chat app implementation
│   └── include/app/       # Chat app headers
├── engine/                # Custom GUI framework
│   ├── include/engine/    # Engine headers (Window, Panel, Events, Layers)
│   └── src/               # Engine implementations
├── network_core/          # Networking library
│   ├── include/           # Network headers (Socket, Connection, Protocol)
│   ├── src/               # Network implementations
│   └── test/              # Network unit tests
└── lib/                   # Third-party libraries
    └── googletest/        # Google Test framework
```

## Installation

Clone the repository with submodules:
```bash
git clone https://github.com/benofthebens/chat-app.git --recurse-submodules
cd chat-app
```

If you already cloned without submodules:
```bash
git submodule update --init --recursive
```

## Building

### Using Visual Studio (MSVC)
```bash
# Configure
cmake --preset x64-debug

# Build
cmake --build out/build/x64-debug
```

For release builds:
```bash
cmake --preset x64-release
cmake --build out/build/x64-release
```

### Using MinGW
```bash
# Configure
cmake --preset mingw-debug

# Build
cmake --build out/build/mingw-debug
```

## Usage

The project builds two executables:

### 1. Chat Application (App.exe)
The main GUI client application with integrated networking.
```bash
./out/build/x64-debug/app/App.exe
```

- Type messages in the text input box
- Click the button to send messages
- Messages from other clients appear in message boxes

### 2. Standalone Server (Server.exe)
A dedicated server for hosting chat sessions.
```bash
./out/build/x64-debug/app/Server.exe
```

The server listens on `127.0.0.1:8080` and broadcasts messages to all connected clients.

### Running a Chat Session

1. Start the server:
```bash
   ./out/build/x64-debug/app/Server.exe
```

2. Launch one or more client instances:
```bash
   ./out/build/x64-debug/app/App.exe
```

3. Clients automatically connect to `127.0.0.1:8080` on startup

## Architecture

### Engine (Custom GUI Framework)
A custom event-driven GUI framework built on Win32 API:

- **Application**: Main application loop and layer management
- **Window**: Top-level window with event callbacks
- **Panel**: Base widget class supporting hierarchical UI
- **Layer**: Application state/screen abstraction
- **Event System**: Type-safe event dispatching (Window, Custom events)
- **GraphicsContext**: Rendering abstraction over Win32 GDI

**Widgets:**
- `Button`: Clickable button with callback support
- `TextInput`: Single-line text input field

### Network Core
Generic networking library with template-based protocol handling:

- **Socket**: Low-level Winsock2 wrapper
- **Connection**: Server and Client connection abstractions
- **ProtocolHandler**: Template-based serialization/deserialization
- **Session**: Manages individual client sessions
- **ApplicationServer/Client**: High-level server and client implementations

### Chat Application
Combines Engine and NetworkCore using a layered architecture:

- **ChatAppLayer**: Handles UI rendering and chat events
- **NetworkLayer**: Manages network communication in separate thread
- **Custom Events**: `MessageSendEvent`, `MessageReceiveEvent`

## Testing

Run the network core tests:
```bash
# Using CTest
ctest --test-dir out/build/x64-debug/network_core/test

# Or run directly
./out/build/x64-debug/network_core/test/NetworkCoreTests.exe
```

## Known Limitations

- Hardcoded server address (127.0.0.1:8080)
- No encryption or authentication
- Windows-only (uses Win32 API)
- Basic error handling
- Fixed message size (1024 bytes)

## Future Improvements

- Configurable server IP/port
- Chat history view with scrollable message list
- User nicknames and avatars
- Persistent message storage
- Reconnection logic
- Cross-platform support (abstract GUI layer)
- Better visual design and UX
- Private messaging

## License

Apache License 2.0 - See [LICENSE.txt](LICENSE.txt) for details.

## Learning Objectives

This project was built to understand:
- How GUI frameworks abstract Win32 API
- Network programming with Winsock2
- Event-driven architectures
- Layer-based application design
- Template metaprogramming for protocols