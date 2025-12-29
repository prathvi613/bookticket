#include <iostream>
#include <string>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {

    // 1. Start Winsock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    // 2. Create socket
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 3. Bind
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (sockaddr*)&address, sizeof(address));

    // 4. Listen
    listen(server_fd, 3);
    cout << "Backend running on http://localhost:8080\n";

    while (true) {

        // 5. Accept client
        int addrlen = sizeof(address);
        SOCKET client = accept(server_fd, (sockaddr*)&address, &addrlen);

        // 6. Receive request
        char buffer[2048] = {0};
        recv(client, buffer, 2048, 0);

        // 7. Simple JSON response
        string body = "{\"ticket_id\":1,\"from\":\"stop a\",\"to\":\"stop b\",\"fare\":20}";
        string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Length: " + to_string(body.length()) +
            "\r\n\r\n" + body;

        // 8. Send
        send(client, response.c_str(), response.length(), 0);

        // 9. Close client socket
        closesocket(client);
    }

    // 10. Cleanup (never reached normally)
    closesocket(server_fd);
    WSACleanup();

    return 0;
}
