#include <iostream>
#include <cmath>

int modExp(int base, int exp, int mod)
{
    int result = 1;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int bruteForceSecretKey(int p, int g, int publicKey)
{
    for (int i = 0; i < p; ++i)
    {
        if (modExp(g, i, p) == publicKey)
        {
            return i; // This is the private key
        }
    }
    return -1; // Failed to find the key
}

int main()
{
    // Diffie-Hellman parameters
    int p = 23; // Prime number
    int g = 9;  // Primitive root modulo p

    // Assume Alice and Bob have these secret keys
    int secretAlice = 4; // This is 'a'
    int secretBob = 3;   // This is 'b'

    // Public keys exchanged
    int publicAlice = modExp(g, secretAlice, p);
    int publicBob = modExp(g, secretBob, p);

    // Attacker intercepts public keys
    std::cout << "Intercepted public keys:\n";
    std::cout << "Public Alice: " << publicAlice << std::endl;
    std::cout << "Public Bob: " << publicBob << std::endl;

    // Attacker performs brute force attack to find secret keys
    int secretKeyAlice = bruteForceSecretKey(p, g, publicAlice);
    int secretKeyBob = bruteForceSecretKey(p, g, publicBob);

    std::cout << "\nAttacker found secret keys:\n";
    std::cout << "Secret key of Alice: " << secretKeyAlice << std::endl;
    std::cout << "Secret key of Bob: " << secretKeyBob << std::endl;

    // Attacker computes the shared secret key using Alice's public key and Bob's secret key
    int sharedSecretAlice = modExp(publicBob, secretKeyAlice, p);
    int sharedSecretBob = modExp(publicAlice, secretKeyBob, p);

    std::cout << "\nShared secret keys computed by attacker:\n";
    std::cout << "Shared secret (via Alice): " << sharedSecretAlice << std::endl;
    std::cout << "Shared secret (via Bob): " << sharedSecretBob << std::endl;

    // Both should be the same if the attack was successful
    if (sharedSecretAlice == sharedSecretBob)
    {
        std::cout << "\nMITM Attack Successful. Shared secret: " << sharedSecretAlice << std::endl;
    }
    else
    {
        std::cout << "\nMITM Attack Failed." << std::endl;
    }

    return 0;
}
