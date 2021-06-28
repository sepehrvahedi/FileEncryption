#include <stdint.h>
#include <string.h>

static uint32_t rotl(uint32_t a, uint32_t b) {
    return (a << b) | (a >> (32 - b));
}

static void qr(uint32_t x[], uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    x[a] = x[a] + x[b];
    x[d] = rotl(x[d] ^ x[a], 16);
    x[c] = x[c] + x[d];
    x[b] = rotl(x[b] ^ x[c], 12);
    x[a] = x[a] + x[b];
    x[d] = rotl(x[d] ^ x[a], 8);
    x[c] = x[c] + x[d];
    x[b] = rotl(x[b] ^ x[c], 7);
}

static void chacha_block(uint32_t out[16], uint32_t const in[16]) {
    uint32_t x[16];
    for (int i = 0; i < 16; ++i)
        x[i] = in[i];
    for (int i = 0; i < 20; i += 2) {
        qr(x, 0, 4, 8, 12);
        qr(x, 1, 5, 9, 13);
        qr(x, 2, 6, 10, 14);
        qr(x, 3, 7, 11, 15);
        qr(x, 0, 5, 10, 15);
        qr(x, 1, 6, 11, 12);
        qr(x, 2, 7, 8, 13);
        qr(x, 3, 4, 9, 14);
    }
    for (int i = 0; i < 16; ++i)
        out[i] = x[i] + in[i];
}

/**
 * Encrypts or decrypts the data with chacha20 block cipher
 * @param data The data to encrypt/decrypt
 * @param key The key of encryption/decryption; Must be 32 bytes
 * @param nonce The nonce of encryption/decryption; Must be 12 bytes
 * @param num_bytes The number of bytes to read from data. MUST BE MULTIPLY OF 64
 */
void encrypt(char *data, const char key[32], const char nonce[12], const int num_bytes) {
    // initialize state
    uint32_t state[16];
    strcpy((char *) state, "expand 32-byte k"); // the constant
    memcpy((char *) (state + 4), key, 32); // the key
    memcpy((char *) (state + 13), nonce, 12); // the nonce
    state[12] = 0; // the position
    // encrypt decrypt
    int offset = 0; // our position in data
    while (num_bytes != offset) { // encrypt until end
        uint32_t out[16]; // result of block
        uint8_t tmp[64]; // result of block but in bytes

        // do the chacha block on state
        chacha_block(out, state);

        // convert ints to bytes
        for (int i = 15; i >= 0; i--) {
            tmp[4 * i] = (uint8_t) out[i];
            tmp[4 * i + 1] = (uint8_t) (out[i] >> 8);
            tmp[4 * i + 2] = (uint8_t) (out[i] >> 16);
            tmp[4 * i + 3] = (uint8_t) (out[i] >> 24);
        }

        // increase position
        state[12]++;

        // set result
        for (int i = 0; i < 64; i++)
            data[i + offset] = (char) ((uint8_t) data[i + offset] ^ tmp[i]);

        // move 64 bytes ahead
        offset += 64;
    }
}

/**
 * Increase nonce by one
 * @param nonce The nonce to increase
 */
void increase_nonce(unsigned char *nonce) {
    nonce[0]++; // increase the first element
    for (int i = 0; i < 11; i++) { // we might increase nonce[12] if (i < 12)
        if (nonce[i] == 0) // overflow! add to next
            nonce[i + 1]++;
        else // no overflow, return the function
            return;
    }
}