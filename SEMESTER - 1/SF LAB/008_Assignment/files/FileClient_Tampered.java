import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import java.net.*;
import java.io.*;
import java.security.KeyPair;

public class FileClient_Tampered {
    public static void main(String[] args) throws Exception {
        String filePath = "C:\\Users\\ssr55\\Desktop\\sf\\sample.txt"; // Path to the file you want to transfer
        String host = "localhost";
        int port = 12345;

        // Generate AES key and IV, then encrypt the file
        SecretKey aesKey = AESCipher.generateAESKey();
        IvParameterSpec iv = AESCipher.generateIV();
        byte[] encryptedData = AESCipher.encryptFile(filePath, aesKey, iv);

        // Generate ECDSA key pair and sign the encrypted file
        KeyPair ecdsaKeyPair = ECDSAKeyPair.generateECDSAKeyPair();
        byte[] signature = ECDSAKeyPair.signFile(encryptedData, ecdsaKeyPair.getPrivate());
        byte[] publicKey = ecdsaKeyPair.getPublic().getEncoded();

        // Start socket connection
        Socket socket = new Socket(host, port);
        DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
        encryptedData[0] ^= 0xFF; // Simulate tampering by flipping a bit

        // Send encrypted file data
        dos.writeInt(encryptedData.length);
        dos.write(encryptedData);

        // Send ECDSA signature
        dos.writeInt(signature.length);
        dos.write(signature);

        // Send public key for verification
        dos.writeInt(publicKey.length);
        dos.write(publicKey);

        // Send AES key and IV (converted to byte arrays)
        dos.writeInt(aesKey.getEncoded().length);
        dos.write(aesKey.getEncoded());
        dos.writeInt(iv.getIV().length);
        dos.write(iv.getIV());

        socket.close();
    }
}
