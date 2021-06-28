#ifndef ENCRYPTOR_CHACHA20_H
#define ENCRYPTOR_CHACHA20_H

#endif //ENCRYPTOR_CHACHA20_H

/**
 * Encrypts or decrypts the data with chacha20 block cipher
 * @param data The data to encrypt/decrypt
 * @param key The key of encryption/decryption; Must be 32 bytes
 * @param nonce The nonce of encryption/decryption; Must be 12 bytes
 * @param num_bytes The number of bytes to read from data. MUST BE MULTIPLY OF 64
 */
void encrypt(char *, const char *, const char *, int);

void increase_nonce(unsigned char *);