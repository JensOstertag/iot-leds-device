#include <AESLib.h>
#include <base64.hpp>

AESLib aesLib;

#define KEY_BYTES 16
#define IV_BYTES 16

byte AES_KEY[KEY_BYTES];
byte AES_IV[IV_BYTES];

void setupEncryption() {
  for(int i = 0; i < KEY_BYTES; i++) {
    if(String(DEV_API_KEY).length() > i) {
      AES_KEY[i] = (byte) DEV_API_KEY[i];
    } else {
      AES_KEY[i] = 0x00;
    }
  }

  for(int i = 0; i < IV_BYTES; i++) {
    if(String(DEV_UID).length() > i) {
      AES_IV[i] = (byte) DEV_UID[i];
    } else {
      AES_IV[i] = 0x00;
    }
  }
}

String decrypt(String encrypted) {
  byte encodedBytes[encrypted.length()];
  encrypted.getBytes(encodedBytes, encrypted.length() + 1);

  byte decodingBuffer[2 * encrypted.length() + 1];
  int decodedLength = decode_base64(encodedBytes, decodingBuffer);
  decodingBuffer[decodedLength] = 0;

  byte decoded[decodedLength];
  for(int i = 0; i < sizeof(decoded); i++) {
    decoded[i] = decodingBuffer[i];
  }

  // -------------------------

  byte decryptingBuffer[sizeof(decoded)];
  int decryptedLength = aesLib.decrypt(decoded, sizeof(decoded), decryptingBuffer, AES_KEY, sizeof(AES_KEY), AES_IV);

  // Remove PKCS#7 padding
  byte paddingLength = decryptingBuffer[sizeof(decryptingBuffer) - 1];
  int length = sizeof(decryptingBuffer) - paddingLength;

  byte plaintext[length + 1];
  for(int i = 0; i < sizeof(plaintext); i++) {
    if(sizeof(decryptingBuffer) > i) {
      plaintext[i] = decryptingBuffer[i];
    }
  }
  plaintext[length] = 0;

  return String((char*) plaintext);
}
