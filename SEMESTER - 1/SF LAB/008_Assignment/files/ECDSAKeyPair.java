import java.security.*;
import java.util.Base64;

public class ECDSAKeyPair {
    public static KeyPair generateECDSAKeyPair() throws Exception {
        KeyPairGenerator keyGen = KeyPairGenerator.getInstance("EC");
        keyGen.initialize(256, new SecureRandom());
        return keyGen.generateKeyPair();
    }

    public static byte[] signFile(byte[] data, PrivateKey privateKey) throws Exception {
        Signature signature = Signature.getInstance("SHA256withECDSA");
        signature.initSign(privateKey);
        signature.update(data);
        return signature.sign();
    }
}
