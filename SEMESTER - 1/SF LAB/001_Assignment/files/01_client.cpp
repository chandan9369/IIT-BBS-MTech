#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 9999

struct sockaddr_in srv;

// Function to receive messages from the server
void ReceiveMessages(int nSocket)
{
    char sBuff[255] = { 0, };
    while (true)
    {
        int err = recv(nSocket, sBuff, sizeof(sBuff) - 1, 0);
        if (err > 0)
        {
            sBuff[err] = '\0'; // Null-terminate the received message
            
            string msg(sBuff);
            string id = "";
            size_t pos = msg.find('#');

            if (pos != string::npos) {
                string id = msg.substr(pos + 1);
                cout << endl << "Message from "<<id<<" : " << msg.substr(0,pos) << endl;
            }
        }
        else if (err == 0)
        {
            cout << endl << "Server disconnected.";
            break;
        }
        else
        {
            cout << endl << "recv failed.";
            break;
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
    {
        cout << endl << "WSAStartup call failed";
        return EXIT_FAILURE;
    }

    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0)
    {
        cout << endl << "Socket creation failed";
        return EXIT_FAILURE;
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr("10.10.126.193"); // Update this IP as needed
    memset(&srv.sin_zero, 0, sizeof(srv.sin_zero));

    int err = connect(nSocket, (struct sockaddr*)&srv, sizeof(srv));
    if (err < 0)
    {
        cout << endl << "Connect failed..";
        return EXIT_FAILURE;
    }

    // Receive and display the client id from the server
    char id[50];
    cout << endl << "Enter your id: ";
    cin.getline(id, sizeof(id));
    err = send(nSocket, id, strlen(id), 0);
    if (err < 0)
    {
        cout << endl << "Send id failed.";
        return EXIT_FAILURE;
    }

    // Start a thread to receive messages from the server
    thread receiveThread(ReceiveMessages, nSocket);
    receiveThread.detach(); // Run the thread in the background

    // Main thread will handle sending messages to the server
    while (true)
    {
        string recipient;
        string message;

        // Input recipient and message
        cout << endl << "Enter recipient id: ";
        getline(cin, recipient);
        cout << endl << "Enter message to send: ";
        getline(cin, message);

        // Format message as "recipient_id:message"
        string formattedMessage = recipient + ':' + message + '#' + id;

        err = send(nSocket, formattedMessage.c_str(), formattedMessage.length(), 0);
        if (err < 0)
        {
            cout << endl << "Send failed.";
            break;
        }
    }

    // Cleanup
    closesocket(nSocket);
    WSACleanup();
    return 0;
}
