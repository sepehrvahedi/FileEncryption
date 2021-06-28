#include <stdlib.h>
#include <string.h>

static unsigned int pw(unsigned int a, unsigned int b) {
    const unsigned int mod = 1000000000 + 7;
    if (b == 0)
        return 1;
    unsigned int ans = a, ind = 1;
    while (ind <= b) {
        ind *= 2;
        if (ind <= b)
            ans = (ans * ans) % mod;
    }
    for (int i = ind / 2; i < b; i++) {
        ans = (ans * a) % mod;
    }
    return ans;
}

static int hash_pass(char s[]) {
    const int mod = 1000000000 + 7;
    unsigned long n = strlen(s);
    int hash = 0;
    for (int i = 0; i < n; i++) {
        int x = pw(256, i);
        int y = (s[i] * x) % mod;
        hash += y % mod;
    }
    return hash;
}

char *key_gen(const char *password) {
    char *key = malloc(32 * sizeof(int));
    int key_int[8]; // at first write in int array, then convert to char array
    const int len = strlen(password);
    const int chunks = len / 8; // length of each chunks
    int offset = 0; // how much we have read
    for (int i = 0; i < 8; i++) {
        const int this_len =
                chunks + (i < (len % 8)); // get length of this chunk. if (i < (len % 8)) then add one to this_len
        char chunk[this_len + 1];
        memcpy(chunk, password + offset, this_len);
        chunk[this_len] = 0;
        key_int[i] = hash_pass(chunk); // write to key
        offset += this_len; // advance in password
    }
    memcpy(key, (char *) key_int, 32); // convert int to char array
    return key;
}