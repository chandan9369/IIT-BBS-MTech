#include <iostream>
#include <WinSock2.h>
#include <map>
#include <string>
#include <cmath>
#include <vector>

#define PORT 9999
#define MAX_CLIENTS 5

struct sockaddr_in srv;

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int nListenerSocket;
int nClientSocket[MAX_CLIENTS];
fd_set fr;
int nMaxFd;

map<int, string> clientId; // Map to store client id
map<string, int> idToSocket; // Map to store id to socket mapping

// RC4 Implementation as defined earlier...
class RC4
{
private:
    vector<int> S;
    int i, j;

public:
    RC4(const string& key) : S(256), i(0), j(0)
    {
        // Key Scheduling Algorithm (KSA)
        for (int i = 0; i < 256; i++)
        {
            S[i] = i;
        }

        int j = 0;
        for (int i = 0; i < 256; i++)
        {
            j = (j + S[i] + key[i % key.length()]) % 256;
            swap(S[i], S[j]);
        }

        i = j = 0;
    }

    char getByte()
    {
        // Pseudo-Random Generation Algorithm (PRGA)
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        return S[(S[i] + S[j]) % 256];
    }

    string encryptDecrypt(const string& data)
    {
        string result = data;
        for (size_t k = 0; k < data.size(); k++)
        {
            result[k] ^= getByte();
        }
        return result;
    }
};

void AcceptNewConnection()
{
    int nIndex;
    int nAvailIndex = -1;
    for (nIndex = 0; nIndex < MAX_CLIENTS; nIndex++)
    {
        if (nClientSocket[nIndex] == 0)
        {
            nAvailIndex = nIndex;
            break;
        }
    }
    if (nAvailIndex == -1)
    {
        cout << endl << "Server Busy..";
        return;
    }

    int nCliSocket = accept(nListenerSocket, NULL, NULL);
    if (nCliSocket < 0)
    {
        cout << endl << "Failed to accept a new client..";
        return;
    }
    nClientSocket[nAvailIndex] = nCliSocket;

    // Receive initial id from client
    char id[50];
    int recvSize = recv(nCliSocket, id, sizeof(id), 0);
    if (recvSize > 0)
    {
        id[recvSize] = '\0';
        clientId[nCliSocket] = string(id);
        idToSocket[string(id)] = nCliSocket;
        cout << endl << "New client connected: " << id;
    }
    else
    {
        cout << endl << "Failed to receive client id.";
    }
}

void UnicastMessage(const char* message, const string& targetid, RC4& rc4)
{
    auto it = idToSocket.find(targetid);
    if (it != idToSocket.end())
    {
        int recipientSocket = it->second;  

        send(recipientSocket, message, strlen(message), 0);
    }
    else
    {
        cout << endl << "Recipient not found: " << targetid;
    }
}

void RecvSendClientMsg(int nCliSocket, RC4& rc4)
{
    char sBuff[500] = { 0, };
    int err = recv(nCliSocket, sBuff, 255, 0);
    if (err <= 0)
    {
        // Connection closed or error
        if (err == 0)
            cout << endl << "Client disconnected: " << nCliSocket;
        else
            cout << endl << "Error at client socket: " << nCliSocket;

        string id = clientId[nCliSocket];
        clientId.erase(nCliSocket);
        idToSocket.erase(id);

        closesocket(nCliSocket);
        for (int nIndex = 0; nIndex < MAX_CLIENTS; nIndex++)
        {
            if (nClientSocket[nIndex] == nCliSocket)
            {
                nClientSocket[nIndex] = 0;
                break;
            }
        }
    }
    else
    {
        // Decrypt the received message
        string encryptedMessage(sBuff);
        string decryptedMessage = rc4.encryptDecrypt(encryptedMessage);

        
        // Parse message to determine recipient and content
        // Format: "recipient_id:message#sender_id"
        string message(decryptedMessage);

        size_t pos = message.find(':');
        if (pos != string::npos)
        {
            string targetid = message.substr(0, pos);
            string actualMessage = message.substr(pos + 1);
            UnicastMessage(sBuff, targetid, rc4);
        }
        else
        {
            cout << endl << "Invalid message format: " << sBuff;
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    WSADATA ws;
    int err = WSAStartup(MAKEWORD(2, 2), &ws);
    if (err == 0)
    {
        cout << endl << "Successfully Initialized socket LIB";
    }
    else
    {
        cout << endl << "Failed to initialize the SOCKET API.";
        return EXIT_FAILURE;
    }

    nListenerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nListenerSocket < 0)
    {
        cout << endl << "Socket creation failed.";
        return EXIT_FAILURE;
    }
    cout << endl << "Socket created successfully.";

    int optval = 1;
    err = setsockopt(nListenerSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
    if (err < 0)
    {
        cout << endl << "Failed to set socket options.";
        return EXIT_FAILURE;
    }
    cout << endl << "Socket options set successfully.";

    u_long nMode = 0;
    err = ioctlsocket(nListenerSocket, FIONBIO, &nMode);
    if (err < 0)
    {
        cout << endl << "Failed to set socket mode.";
        return EXIT_FAILURE;
    }
    cout << endl << "Socket mode set to blocking.";

    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = INADDR_ANY;
    memset(&srv.sin_zero, 0, sizeof(srv.sin_zero));
    err = bind(nListenerSocket, (struct sockaddr*)&srv, sizeof(srv));
    if (err < 0)
    {
        cout << endl << "Failed to bind to local port.";
        return EXIT_FAILURE;
    }
    cout << endl << "Bind to local port successful.";

    err = listen(nListenerSocket, MAX_CLIENTS);
    if (err < 0)
    {
        cout << endl << "Failed to listen.";
        return EXIT_FAILURE;
    }
    cout << endl << "Listening on port.";

    nMaxFd = nListenerSocket;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    // Initialize RC4 with a key
    string key = "your_secure_key"; // Replace with your secure key
    RC4 rc4(key);

    while (1)
    {
        FD_ZERO(&fr);
        FD_SET(nListenerSocket, &fr);
        for (int nIndex = 0; nIndex < MAX_CLIENTS; nIndex++)
        {
            if (nClientSocket[nIndex] != 0)
            {
                FD_SET(nClientSocket[nIndex], &fr);
            }
        }

        err = select(nMaxFd + 1, &fr, NULL, NULL, &tv);
        if (err > 0)
        {
            if (FD_ISSET(nListenerSocket, &fr))
            {
                AcceptNewConnection();
            }
            for (int nIndex = 0; nIndex < MAX_CLIENTS; nIndex++)
            {
                if (nClientSocket[nIndex] != 0 && FD_ISSET(nClientSocket[nIndex], &fr))
                {
                    RecvSendClientMsg(nClientSocket[nIndex], rc4);
                }
            }
        }
        else if (err == 0)
        {
            // No need to print anything for this case
        }
        else
        {
            cout << endl << "Select failed, closing server.";
            return EXIT_FAILURE;
        }
    }

    return 0;
}
