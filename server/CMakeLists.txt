set(SERVER_SOURCES src/main.cpp src/tcp_server.cpp)

add_executable(Server ${SERVER_SOURCES})
target_include_directories(Server PRIVATE include)