import java.net.*;
import java.io.*;
import java.nio.file.*;
import java.security.KeyFactory;
import java.security.PublicKey;
import java.security.spec.X509EncodedKeySpec;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class FileServer {
    public static void main(String[] args) throws Exception {
        int port = 12345;
        ServerSocket serverSocket = new ServerSocket(port);
        System.out.println("\n\nServer started, Listening for connections...\n");

        while (true) {
            Socket socket = serverSocket.accept();
            System.out.println("\nClient connected.");

            DataInputStream dis = new DataInputStream(socket.getInputStream());

            // Receive the encrypted file data
            int length = dis.readInt();
            byte[] encryptedData = new byte[length];
            dis.readFully(encryptedData);

            // Receive the signature
            length = dis.readInt();
            byte[] signature = new byte[length];
            dis.readFully(signature);

            // Receive the public key for verification
            length = dis.readInt();
            byte[] publicKeyBytes = new byte[length];
            dis.readFully(publicKeyBytes);

            // Receive the AES key
            length = dis.readInt();
            byte[] aesKeyBytes = new byte[length];
            dis.readFully(aesKeyBytes);

            // Receive the IV
            length = dis.readInt();
            byte[] ivBytes = new byte[length];
            dis.readFully(ivBytes);

            // Verify the signature to ensure authenticity
            boolean isAuthentic = SignatureVerifier.verifySignature(encryptedData, signature, publicKeyBytes);
            if (isAuthentic) {
                System.out.println("\nFile received is authentic.");

                // Convert AES key and IV to usable objects
                SecretKey aesKey = new SecretKeySpec(aesKeyBytes, "AES");
                IvParameterSpec iv = new IvParameterSpec(ivBytes);

                // Decrypt the encrypted file data
                System.out.println("\nEncrypted file saved as 'received_encrypted_file.txt'");
                Files.write(Paths.get("received_encrypted_file.txt"), encryptedData);
                byte[] decryptedData = AESCipher.decryptFile(encryptedData, aesKey, iv);

                // Save decrypted data to a file
                Files.write(Paths.get("decrypted_file.txt"), decryptedData);
                System.out.println("\nDecrypted file saved as 'decrypted_file.txt'");
            } else {
                // case when file is tampered
                System.out.println("\nFile tampering detected!");
            }

            socket.close();
        }
    }
}
