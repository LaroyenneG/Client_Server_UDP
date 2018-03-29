cmake_minimum_required(VERSION 3.9)
project(Client_Server_UDP)

set(CMAKE_CXX_STANDARD 17)

add_executable(client
        client.c client_server.h)

add_executable(server
        server.c client_server.h)