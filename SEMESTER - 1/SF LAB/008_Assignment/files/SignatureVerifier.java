import java.security.*;
import java.security.spec.*;

public class SignatureVerifier {
    public static boolean verifySignature(byte[] data, byte[] signature, byte[] publicKey) throws Exception {
        KeyFactory keyFactory = KeyFactory.getInstance("EC");
        X509EncodedKeySpec keySpec = new X509EncodedKeySpec(publicKey);
        PublicKey pubKey = keyFactory.generatePublic(keySpec);

        Signature sig = Signature.getInstance("SHA256withECDSA");
        sig.initVerify(pubKey);
        sig.update(data);

        return sig.verify(signature);
    }
}
