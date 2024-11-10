#include "Function.h"
using namespace std;

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    cout << "IPv4 Address: 192.168.174.130\n";
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error create Winsock!" << endl;
        return 1;
    }


    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error create socket!" << endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);


    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error connected socket!" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }


    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Error during listening" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }


    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error accepting connection!" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1; 
    }
    cout << "The Server has successfully accepted the connection to the Client\n";
    while (true)
    {
        string buffer(1024, '\0');
        recv(clientSocket, &buffer[0], buffer.size(), 0);
        cout << "Data received from the client: " << buffer << endl;

        if (buffer.find("shutdown") != string::npos)
        {
            shutdownServer();
        }
        else if (buffer.find("reset") != string::npos)
        {
            resetServer();
        }
        string message = "Mission Complete";
        send(clientSocket, message.c_str(), message.length() + 1, 0);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}