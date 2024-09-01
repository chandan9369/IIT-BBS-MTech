#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 9999

struct sockaddr_in srv;

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

// Function to receive messages from the server
void ReceiveMessages(int nSocket, RC4& rc4)
{
    char sBuff[500] = { 0 };
    while (true)
    {
        int err = recv(nSocket, sBuff, sizeof(sBuff) - 1, 0);
        if (err > 0)
        {
            sBuff[err] = '\0'; // Null-terminate the received message

            // Decrypt the received message
            string encryptedMessage(sBuff);
            string decryptedMessage = rc4.encryptDecrypt(encryptedMessage);

            size_t pos1 = decryptedMessage.find(':');

            cout <<endl<<endl<< decryptedMessage << endl << endl;
            if (pos1 != string::npos)
            {
     
                size_t pos = decryptedMessage.find('#');

                if (pos != string::npos)
                {
                    string senderId = decryptedMessage.substr(pos + 1);
                    string actualMsg = decryptedMessage.substr(pos1 + 1, pos);

                    cout << endl
                        << "Encrypted message from " << senderId << " : " << sBuff << endl;
                    cout << endl
                        << "Decrypted message from " << senderId << " : " << actualMsg << endl;
                }
            }
        }
        else if (err == 0)
        {
            cout << endl
                << "Server disconnected.";
            break;
        }
        else
        {
            cout << endl
                << "recv failed.";
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
        cout << endl
            << "WSAStartup call failed";
        return EXIT_FAILURE;
    }

    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0)
    {
        cout << endl
            << "Socket creation failed";
        return EXIT_FAILURE;
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr("10.10.126.193"); // Update this IP as needed
    memset(&srv.sin_zero, 0, sizeof(srv.sin_zero));

    int err = connect(nSocket, (struct sockaddr*)&srv, sizeof(srv));
    if (err < 0)
    {
        cout << endl
            << "Connect failed..";
        return EXIT_FAILURE;
    }

    // Receive and display the client id from the server
    char id[50];
    cout << endl
        << "Enter your id: ";
    cin.getline(id, sizeof(id));
    err = send(nSocket, id, strlen(id), 0);
    if (err < 0)
    {
        cout << endl
            << "Send id failed.";
        return EXIT_FAILURE;
    }

    // Initialize RC4 with a key
    string key = "your_secure_key"; // Replace with your key
    RC4 rc4(key);

    // Start a thread to receive messages from the server
    thread receiveThread(ReceiveMessages, nSocket, ref(rc4));
    receiveThread.detach(); // Run the thread in the background

    // Main thread will handle sending messages to the server
    while (true)
    {
        string recipient;
        string message;

        // Input recipient and message
        cout << endl
            << "Enter recipient id: ";
        getline(cin, recipient);
        cout << endl
            << "Enter message to send: ";
        getline(cin, message);

        // Format message as "recipient_id:message"
        string formattedMessage = recipient + ':' + message + '#' + id;

        // Encrypt the message
        string encryptedMessage = rc4.encryptDecrypt(formattedMessage);

        err = send(nSocket, encryptedMessage.c_str(), encryptedMessage.length(), 0);
        if (err < 0)
        {
            cout << endl
                << "Send failed.";
            break;
        }
    }

    // Cleanup
    closesocket(nSocket);
    WSACleanup();
    return 0;
}
