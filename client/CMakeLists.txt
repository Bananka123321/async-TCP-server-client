set(CLIENT_SOURCES src/main.cpp src/tcp_client.cpp)

add_executable(Client ${CLIENT_SOURCES})
target_include_directories(Client PRIVATE include)