#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#define KEYSIZE 16
#define BLOCKSIZE 16

void hex_to_bytes(const char* hex_string, unsigned char* byte_array) {
    size_t len = strlen(hex_string);
    for (size_t i = 0; i < len / 2; i++) {
        sscanf(hex_string + 2 * i, "%2hhx", &byte_array[i]);
    }
}

void generate_key(time_t seed, unsigned char* key) {
    srand(seed);
    for (int i = 0; i < KEYSIZE; i++) {
        key[i] = rand() % 256;
    }
}

int encrypt_aes_128_cbc(unsigned char* key, unsigned char* iv, unsigned char* plaintext, unsigned char* ciphertext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return 0;
    int len;
    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1) return 0;
    if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, BLOCKSIZE) != 1) return 0;
    EVP_CIPHER_CTX_free(ctx);
    return 1;
}

int compare_blocks(unsigned char* generated_block, unsigned char* known_block) {
    return memcmp(generated_block, known_block, BLOCKSIZE) == 0;
}

int main() {
    unsigned char plaintext[BLOCKSIZE], known_ciphertext[BLOCKSIZE], iv[BLOCKSIZE], generated_key[KEYSIZE], generated_ciphertext[BLOCKSIZE];

    hex_to_bytes("255044462d312e350a25d0d4c5d80a34", plaintext);
    hex_to_bytes("d06bf9d0dab8e8ef880660d2af65aa82", known_ciphertext);
    hex_to_bytes("09080706050403020100A2B2C2D2E2F2", iv);

    long long base_timestamp = 1524004129;
    time_t start_time = base_timestamp - 24 * 60 * 60, end_time = base_timestamp + 24 * 60 * 60;

    for (time_t current_time = start_time; current_time <= end_time; current_time++) {
        generate_key(current_time, generated_key);

        if (!encrypt_aes_128_cbc(generated_key, iv, plaintext, generated_ciphertext)) {
            printf("Erreur de chiffrement\n");
            return 1;
        }

        if (compare_blocks(generated_ciphertext, known_ciphertext)) {
            printf("Clé trouvée ! Timestamp : %lld\nClé : ", (long long)current_time);
            for (int i = 0; i < KEYSIZE; i++) printf("%.2x", generated_key[i]);
            printf("\n");
            return 0;
        }
    }

    printf("Clé non trouvée dans la fenêtre de 24 heures.\n");
    return 1;
}