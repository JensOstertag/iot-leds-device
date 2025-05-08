#include <AESLib.h>

AESLib aesLib;

byte AES_KEY[KEY_BYTES];
byte AES_IV[IV_BYTES];

void setupEncryption() {
  for(int i = 0; i < KEY_BYTES; i++) {
    if(String(DEV_API_KEY).length() >= i - 1) {
      AES_KEY[i] = (byte) DEV_API_KEY[i];
    } else {
      AES_KEY[i] = 0x00;
    }
  }

  for(int i = 0; i < IV_BYTES; i++) {
    if(String(DEV_UID).length() >= i - 1) {
      AES_IV[i] = (byte) DEV_UID[i];
    } else {
      AES_IV[i] = 0x00;
    }
  }
}

String decodeBase64(String encoded) {
  byte encodedBytes[encoded.length()];
  encoded.getBytes(encodedBytes, encoded.length() + 1);

  byte decodingBuffer[encoded.length() + 1];
  int decodedLength = decode_base64(encodedBytes, decodingBuffer);
  decodingBuffer[decodedLength] = 0;

  byte decoded[decodedLength + 1];
  for(int i = 0; i < sizeof(decoded); i++) {
    decoded[i] = decodingBuffer[i];
  }

  return String((char*) decoded);
}

String decryptAes(String ciphertext) {
  byte ciphertextBytes[ciphertext.length() + 1];
  ciphertext.getBytes(ciphertextBytes, ciphertext.length() + 1);

  byte decryptingBuffer[ciphertext.length()];
  int decryptedLength = aesLib.decrypt(ciphertextBytes, sizeof(ciphertextBytes), decryptingBuffer, aes_key, sizeof(aes_key), aes_iv);

  int length = sizeof(decryptingBuffer);
  for(int i = sizeof(decryptingBuffer) - 1; i >= 0; i--) {
    if(decryptingBuffer[i] == 0x0F) {
      length--;
    } else {
      break;
    }
  }

  byte plaintext[length + 1];
  for(int i = 0; i < sizeof(plaintext); i++) {
    plaintext[i] = decryptingBuffer[i];
  }
  plaintext[length] = 0;

  return String((char*) plaintext);
}
