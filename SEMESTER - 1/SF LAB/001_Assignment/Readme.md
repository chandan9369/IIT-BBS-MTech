## QUESTION 1: Socket Programming

### Task:

-  Establish a connection between clients and a server, allowing them to communicate by sending messages.

### Steps:

1. Open the sample_server.cpp in Visual Studio 2022.
2. Compile the server code:
   -  Navigate to Build > Build Solution or press Ctrl+Shift+B.
3. Run the server:
   -  Press Ctrl+F5 to run without debugging.
4. Open the sample_client.cpp in a new Visual Studio 2022 terminal.
5. Compile the client code:
   -  Follow the same steps as for the server.
6. Run the client:
   -  Again, press Ctrl+F5 to run without debugging.

### Outcome:

A connection is established between each client and the server independently, allowing them to communicate by sending messages through the terminal.

## QUESTION 2: Diffie-Hellman Key Exchange

### Task:

Compute a shared key between client and server using the Diffie-Hellman key exchange method.

### Steps:

1. Open the diffie_server.cpp in Visual Studio 2022.
2. Compile the server code as before.
3. Run the server:
   -  Input the private key b for the server when prompted.
4. Open the diffie_client.cpp in a new terminal window in Visual Studio 2022.
5. Compile the client code.
6. Run the client:
   -  Input the private key for the client when prompted.

### Outcome:

The shared key is computed and displayed on both the server and client terminals.

## QUESTION 3: RC4 Encryption/Decryption

### Task:

Encrypt and decrypt a message using the RC4 algorithm.

### Steps:

1. Open the server_rc4.cpp in Visual Studio 2022.
2. Compile the server code.
3. Run the server.
4. Open the client_rc4.cpp in a new terminal window in Visual Studio 2022.
5. Compile the client code.
6. Run the client:
   -  Enter the message to be encrypted when prompted.

### Outcome:

The message is encrypted on the client side, sent to the server, and then decrypted and displayed on the server terminal.

## QUESTION 4: Man-In-The-Middle (MITM) Attack Simulation

### Task:

Simulate a Man-In-The-Middle (MITM) attack where a proxy intercepts communication between a client and server.

### Steps:

1. Open the 04_mim_attack.cpp in terminal.
2. Compile the code by entering `g++ 04_mim_attack.cpp -o 04_mim_attack"
3. We will have our result.
