# cmake needs this line
cmake_minimum_required(VERSION 3.1)

# Define project name
project(test_client)


set(SOURCE SocketClient.cpp test_client.cpp)
set(HEADERS SocketClient.h)
# Declare the executable target built from your sources
add_executable(test_client ${SOURCE})
